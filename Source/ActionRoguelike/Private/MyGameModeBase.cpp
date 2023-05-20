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
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "SInteractionInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "MyMonsterData.h"
#include "../ActionRoguelike.h"
#include "Actions/MyActionComponent.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AMyGameModeBase::AMyGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPickupCount = 10;
	RequiredPickupDistance = 2000;

	PlayerStateClass = AMyPlayerState::StaticClass();
	SlotName = "SaveGame01";
}

void AMyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	// ����Ϸ��ʼ��ʱ������Ϸ�浵
	LoadSaveGame();
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
		// �����������
		if (MonsterTable)
		{
			// ����һ��MonsterInfoRowָ�������
			TArray<FMonsterInfoRow*> Rows;
			// �ӹ�����л�ȡ�����У��������Rows����
			MonsterTable->GetAllRows("", Rows);

			// ���ѡ��һ������
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];
			// ��ȡ�ʲ�������ʵ��
			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				// ������ع������ʾ��Ϣ
				LogOnScreen(this, "Loading monster...", FColor::Green);
				// ������ع������ʾ��Ϣ
				TArray<FName> Bundles;
				// ����һ���ص�����
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AMyGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				// ʹ���ʲ�����������ѡ��Ĺ���
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}
void AMyGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	// ��ȡ�ʲ�������ʵ��
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		// ͨ����������ȡ���ʲ����󣨼�����������Դ��
		UMyMonsterData* MonsterData = Cast<UMyMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			// ͨ������������Դ����Ϣ����һ���µĹ����ɫ
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				// ��������¹������ʾ��Ϣ
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// ��ȡ�����ɫ����Ϊ���
				UMyActionComponent* ActionComp = Cast<UMyActionComponent>(NewBot->GetComponentByClass(UMyActionComponent::StaticClass()));
				if (ActionComp)
				{
					// Ϊ�����ɫ��������е���Ϊ
					for (TSubclassOf<UMyAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
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

		float RespawnDelay = 3.0f;
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

void AMyGameModeBase::WriteSaveGame()
{
	// ����GameState�е�PlayerArray
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AMyPlayerState* PS = Cast<AMyPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			// ����ɹ���ȡ��PlayerState������PS��SavePlayerState���������뵱ǰ��SaveGame����
			PS->SavePlayerState(CurrentSaveGame);
			break; // Ŀǰֻ֧�ֵ�����Ϸ�����Դ���һ����Һ���˳�ѭ��
		}
	}
	// ���SavedActors���飬��������ǰ�����Ӱ��
	CurrentSaveGame->SavedActors.Empty();
	// �������������Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// ֻ����ʵ��USInteractionInterface�ӿڵ�"��Ϸ��Actor"
		if (!Actor->Implements<USInteractionInterface>())
		{
			continue;
		}
		// Ϊÿ��Actor����һ��FActorSaveDataʵ��
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();
				
		// ��ActorData��ByteData���鴫�ݸ��ڴ滺����MemWriter
		FMemoryWriter MemWriter(ActorData.ByteData);
		// ����һ���Զ��������Ϊ�ַ����Ĵ���浵
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// ��ʾ��ǰ�ķ����л�������Ϊ�˼�����Ϸ
		Ar.ArIsSaveGame = true;
		// ������SaveGame��ǩ��UPROPERTYת��Ϊ����������
		Actor->Serialize(Ar);
		// ��ActorData��ӵ�CurrentSaveGame��SavedActors������
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	// ����ǰ��Ϸ�浵���󱣴浽ָ���Ĳ�λ
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void AMyGameModeBase::LoadSaveGame()
{
	// �ж�ָ����λ�Ƿ���ڴ浵
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		// ��ָ����λ������Ϸ�浵
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		// �������������Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// ֻ����ʵ��USInteractionInterface�ӿڵ�"��Ϸ��Actor"
			if (!Actor->Implements<USInteractionInterface>())
			{
				continue;
			}
			// ���������Actor����
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				// ���Actor�����Ƿ�ƥ��
				if (ActorData.ActorName == Actor->GetName())
				{
					// ����Actor��λ�á���ת������
					Actor->SetActorTransform(ActorData.Transform);
					// ��ActorData��ByteData���鴫�ݸ��ڴ滺����MemWriter
					FMemoryReader MemReader(ActorData.ByteData);
					// ����һ���Զ��������Ϊ�ַ����Ĵ���浵
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					// ��ʾ��ǰ�ķ����л�������Ϊ�˼�����Ϸ
					Ar.ArIsSaveGame = true;
					// ������SaveGame��ǩ��UPROPERTYת��Ϊ����������
					Actor->Serialize(Ar);
					// ����Actor��λ�á���ת������
					ISInteractionInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		// ��ָ����λ�����ڴ浵���򴴽��µĴ浵����
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
//��������Ҽ�����Ϸ���߼�������������ӵ���Ϸ������ʱ������������ô˺���
void AMyGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// ��ȡNewPlayer��PlayerState�����洢��PS������
	AMyPlayerState* PS = NewPlayer->GetPlayerState<AMyPlayerState>();
	if (ensure(PS))
	{
		//����PS��LoadPlayerState����,���뵱ǰ��SaveGame����
		PS->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}