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
	// 在游戏初始化时加载游戏存档
	LoadSaveGame();
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
		// 如果怪物表存在
		if (MonsterTable)
		{
			// 定义一个MonsterInfoRow指针的数组
			TArray<FMonsterInfoRow*> Rows;
			// 从怪物表中获取所有行，将其存入Rows数组
			MonsterTable->GetAllRows("", Rows);

			// 随机选择一个怪物
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];
			// 获取资产管理器实例
			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				// 输出加载怪物的提示信息
				LogOnScreen(this, "Loading monster...", FColor::Green);
				// 输出加载怪物的提示信息
				TArray<FName> Bundles;
				// 创建一个回调代理
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AMyGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				// 使用资产管理器加载选择的怪物
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}
void AMyGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	// 获取资产管理器实例
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		// 通过管理器获取主资产对象（即怪物数据资源）
		UMyMonsterData* MonsterData = Cast<UMyMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			// 通过怪物数据资源的信息生成一个新的怪物角色
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				// 输出生成新怪物的提示信息
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// 获取怪物角色的行为组件
				UMyActionComponent* ActionComp = Cast<UMyActionComponent>(NewBot->GetComponentByClass(UMyActionComponent::StaticClass()));
				if (ActionComp)
				{
					// 为怪物角色添加其特有的行为
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

		float RespawnDelay = 3.0f;
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

void AMyGameModeBase::WriteSaveGame()
{
	// 遍历GameState中的PlayerArray
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AMyPlayerState* PS = Cast<AMyPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			// 如果成功获取到PlayerState，调用PS的SavePlayerState方法，传入当前的SaveGame对象
			PS->SavePlayerState(CurrentSaveGame);
			break; // 目前只支持单人游戏，所以处理一个玩家后就退出循环
		}
	}
	// 清空SavedActors数组，避免受先前保存的影响
	CurrentSaveGame->SavedActors.Empty();
	// 遍历整个世界的Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// 只关心实现USInteractionInterface接口的"游戏内Actor"
		if (!Actor->Implements<USInteractionInterface>())
		{
			continue;
		}
		// 为每个Actor创建一个FActorSaveData实例
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();
				
		// 将ActorData的ByteData数组传递给内存缓冲区MemWriter
		FMemoryWriter MemWriter(ActorData.ByteData);
		// 创建一个以对象和名称为字符串的代理存档
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// 表示当前的反序列化操作是为了加载游戏
		Ar.ArIsSaveGame = true;
		// 将带有SaveGame标签的UPROPERTY转换为二进制数组
		Actor->Serialize(Ar);
		// 将ActorData添加到CurrentSaveGame的SavedActors数组中
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	// 将当前游戏存档对象保存到指定的槽位
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void AMyGameModeBase::LoadSaveGame()
{
	// 判断指定槽位是否存在存档
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		// 从指定槽位加载游戏存档
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		// 遍历整个世界的Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// 只关心实现USInteractionInterface接口的"游戏内Actor"
			if (!Actor->Implements<USInteractionInterface>())
			{
				continue;
			}
			// 遍历保存的Actor数据
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				// 检查Actor名称是否匹配
				if (ActorData.ActorName == Actor->GetName())
				{
					// 设置Actor的位置、旋转和缩放
					Actor->SetActorTransform(ActorData.Transform);
					// 将ActorData的ByteData数组传递给内存缓冲区MemWriter
					FMemoryReader MemReader(ActorData.ByteData);
					// 创建一个以对象和名称为字符串的代理存档
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					// 表示当前的反序列化操作是为了加载游戏
					Ar.ArIsSaveGame = true;
					// 将带有SaveGame标签的UPROPERTY转换为二进制数组
					Actor->Serialize(Ar);
					// 设置Actor的位置、旋转和缩放
					ISInteractionInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		// 若指定槽位不存在存档，则创建新的存档对象
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
//处理新玩家加入游戏的逻辑。当新玩家连接到游戏服务器时，服务器会调用此函数
void AMyGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 获取NewPlayer的PlayerState，并存储到PS变量中
	AMyPlayerState* PS = NewPlayer->GetPlayerState<AMyPlayerState>();
	if (ensure(PS))
	{
		//调用PS的LoadPlayerState方法,传入当前的SaveGame对象
		PS->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}