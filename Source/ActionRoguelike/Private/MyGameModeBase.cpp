// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/MyAICharacter.h"
#include "MyAttributeComponent.h"
#include "EngineUtils.h"

AMyGameModeBase::AMyGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
	//������һ����ʱ��TimerHandle_SpawnBots����ʱ����SpawnBotTimerElapsed()������ʱ����ΪSpawnTimerInterval�룬ѭ��ִ��
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMyGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AMyGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots = 0;
	//ͳ�Ƶ�ǰ�����д��� AMyAICharacter ���͵Ľ�ɫ����
	for (TActorIterator<AMyAICharacter> It(GetWorld()); It; ++It)
	{
		AMyAICharacter* Bot = *It;

		UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(Bot->GetComponentByClass(UMyAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

	float MaxBotCount = 10.0f;

	//���� DifficultyCurve ���ߵ�ֵ��������� bot ����
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	//�뵱ǰ���� bot �������бȽϡ�������� bot ����С�������������Ӳ�ѯ����л�ȡһ��λ��
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	//����һ��������ѯ��Environment Query System, EQS������ȡ��ѯʵ�� QueryInstance��
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		//����ǰ����󶨵���ѯ����¼�����ѯ��ɺ����� OnQueryCompleted() ������
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnQueryCompleted);
	}
}


void AMyGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	//ͨ�� GetWorld()->SpawnActor<AActor>() �����ڸ�λ������һ�� MinionClass ���͵Ľ�ɫ
	if (Locations.IsValidIndex(0))
	{

		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}