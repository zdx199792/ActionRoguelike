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
	//设置了一个定时器TimerHandle_SpawnBots，定时调用SpawnBotTimerElapsed()函数，时间间隔为SpawnTimerInterval秒，循环执行
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMyGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
		
	// 确保至少有一个掉落物类
	if (ensure(PickupClasses.Num() > 0))
	{
		// 运行环境查询系统（EQS）来查找潜在的掉落物生成位置
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PickupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnPickupSpawnQueryCompleted);
		}
	}
}

void AMyGameModeBase::SpawnBotTimerElapsed()
{
	// 在游戏线程上获取 CVarSpawnBots 控制台变量的值
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		// 如果 CVarSpawnBots 的值为 false（禁止生成机器人），则打印一条警告日志，直接返回，不执行后续的生成机器人操作。
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	//统计当前世界中存活的 AMyAICharacter 类型的角色数量
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

	//根据 DifficultyCurve 曲线的值计算出最大的 bot 数量
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	//与当前存活的 bot 数量进行比较。如果存活的 bot 数量小于最大数量，则从查询结果中获取一个位置
	if (NrOfAliveBots >= MaxBotCount)
	{
		//UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	//运行一个环境查询（Environment Query System, EQS）并获取查询实例 QueryInstance。
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		//将当前对象绑定到查询完成事件，查询完成后会调用 OnQueryCompleted() 函数。
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
	 // 如果 EQS 查询失败，打印警告日志并退出函数
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	// 获取 EQS 查询结果中的所有位置
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	// 用于记录已使用的位置，便于后面检查位置间距
	TArray<FVector> UsedLocations;
	//道具数量
	int32 SpawnCounter = 0;
	// 如果还没有生成足够数量的道具且仍有可用位置，则继续生成道具
	while (SpawnCounter < DesiredPickupCount && Locations.Num() > 0)
	{
		// 从剩余的位置中随机选取一个位置
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		// 将已选位置从数组中删除，避免重复选取
		Locations.RemoveAt(RandomLocationIndex);

		// 检查位置间距是否符合要求
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
		// 如果位置间距不符合要求，跳过该位置
		if (!bValidLocation)
		{
			continue;
		}
		// 随机选取一种道具类别
		int32 RandomClassIndex = FMath::RandRange(0,PickupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPickupClass = PickupClasses[RandomClassIndex];
		// 在随机位置生成道具
		GetWorld()->SpawnActor<AActor>(RandomPickupClass, PickedLocation, FRotator::ZeroRotator);
		// 记录该位置，便于后面检查位置间距
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

    // 杀死目标获得 Credits
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
        // 获取击杀者的 AMyPlayerState 组件，并给予击杀者 Credits 奖励
		if (AMyPlayerState* PS = KillerPawn->GetPlayerState<AMyPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}