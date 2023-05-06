// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/MyAICharacter.h"
#include "MyAttributeComponent.h"
#include "EngineUtils.h"
#include "MyCharacter.h"
#include "MyPlayerState.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AMyGameModeBase::AMyGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPickupCount = 10;
	RequiredPickupDistance = 2000;

	PlayerStateClass = AMyPlayerState::StaticClass();
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
	//������һ����ʱ��TimerHandle_SpawnBots����ʱ����SpawnBotTimerElapsed()������ʱ����ΪSpawnTimerInterval�룬ѭ��ִ��
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMyGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
		
	// ȷ��������һ����������
	if (ensure(PickupClasses.Num() > 0))
	{
		// ���л�����ѯϵͳ��EQS��������Ǳ�ڵĵ���������λ��
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PickupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnPickupSpawnQueryCompleted);
		}
	}
}

void AMyGameModeBase::SpawnBotTimerElapsed()
{
	// ����Ϸ�߳��ϻ�ȡ CVarSpawnBots ����̨������ֵ
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		// ��� CVarSpawnBots ��ֵΪ false����ֹ���ɻ����ˣ������ӡһ��������־��ֱ�ӷ��أ���ִ�к��������ɻ����˲�����
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	//ͳ�Ƶ�ǰ�����д��� AMyAICharacter ���͵Ľ�ɫ����
	for (TActorIterator<AMyAICharacter> It(GetWorld()); It; ++It)
	{
		AMyAICharacter* Bot = *It;

		//UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(Bot->GetComponentByClass(UMyAttributeComponent::StaticClass()));
		UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

	float MaxBotCount = 10.0f;

	//���� DifficultyCurve ���ߵ�ֵ��������� bot ����
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	//�뵱ǰ���� bot �������бȽϡ�������� bot ����С�������������Ӳ�ѯ����л�ȡһ��λ��
	if (NrOfAliveBots >= MaxBotCount)
	{
		//UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	//����һ��������ѯ��Environment Query System, EQS������ȡ��ѯʵ�� QueryInstance��
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		//����ǰ����󶨵���ѯ����¼�����ѯ��ɺ����� OnQueryCompleted() ������
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void AMyGameModeBase::KillAll()
{
	for (TActorIterator<AMyAICharacter> It(GetWorld()); It; ++It)
	{
		AMyAICharacter* Bot = *It;

		UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); 
		}
	}
}
void AMyGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}


void AMyGameModeBase::OnPickupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	 // ��� EQS ��ѯʧ�ܣ���ӡ������־���˳�����
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	// ��ȡ EQS ��ѯ����е�����λ��
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	// ���ڼ�¼��ʹ�õ�λ�ã����ں�����λ�ü��
	TArray<FVector> UsedLocations;
	//��������
	int32 SpawnCounter = 0;
	// �����û�������㹻�����ĵ��������п���λ�ã���������ɵ���
	while (SpawnCounter < DesiredPickupCount && Locations.Num() > 0)
	{
		// ��ʣ���λ�������ѡȡһ��λ��
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		// ����ѡλ�ô�������ɾ���������ظ�ѡȡ
		Locations.RemoveAt(RandomLocationIndex);

		// ���λ�ü���Ƿ����Ҫ��
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPickupDistance)
			{
				bValidLocation = false;
				break;
			}
		}
		// ���λ�ü�಻����Ҫ��������λ��
		if (!bValidLocation)
		{
			continue;
		}
		// ���ѡȡһ�ֵ������
		int32 RandomClassIndex = FMath::RandRange(0,PickupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPickupClass = PickupClasses[RandomClassIndex];
		// �����λ�����ɵ���
		GetWorld()->SpawnActor<AActor>(RandomPickupClass, PickedLocation, FRotator::ZeroRotator);
		// ��¼��λ�ã����ں�����λ�ü��
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}
void AMyGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AMyGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AMyCharacter* Player = Cast<AMyCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 5.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

    // ɱ��Ŀ���� Credits
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
        // ��ȡ��ɱ�ߵ� AMyPlayerState ������������ɱ�� Credits ����
		if (AMyPlayerState* PS = KillerPawn->GetPlayerState<AMyPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}