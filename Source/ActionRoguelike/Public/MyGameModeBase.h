// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "MyGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UMySaveGame;
class UDataTable;
class UMyMonsterData;

// 定义一个结构体FMonsterInfoRow，继承自FTableRowBase，是一个表示怪物信息的数据表
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 默认构造函数
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	// 定义一个名为MonsterId的字段，类型为FPrimaryAssetId，该字段用于标识怪物
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	// 定义一个名为Weight的字段，类型为float，该字段表示选择此怪物的相对机会
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
	// 定义一个名为SpawnCost的字段，类型为float，该字段表示游戏模式生成此单位所需的点数
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	// 定义一个名为KillReward的字段，类型为float，该字段表示杀死此单位的奖励金额
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// 用于保存游戏存档的槽位名称
	FString SlotName;
	// 当前的游戏存档对象
	UPROPERTY()
	UMySaveGame* CurrentSaveGame;


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	//指定要生成的AI角色的类
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;

	//指定用于生成AI角色的环境查询系统（EQS）查询
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	//指定游戏的难度曲线(AI角色生成数量)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	//管理定时生成AI角色的计时器
	FTimerHandle TimerHandle_SpawnBots;

	//指定生成AI角色的时间间隔
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	//杀死AI机器人的得分奖励值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	//生成拾取物的EQS查询
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* PickupSpawnQuery;

	//拾取物类数组
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	//拾取物之间的间距
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float RequiredPickupDistance;

	//拾取物数量上限
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 DesiredPickupCount;

	//计时器回调函数，用于生成AI角色
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPickupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

public:
	AMyGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
	// 重写AGameModeBase的InitGame方法，用于游戏初始化
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// 将游戏数据写入存档
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	// 加载游戏存档的函数
	void LoadSaveGame();
	//处理新玩家加入游戏的逻辑。当新玩家连接到游戏服务器时，服务器会调用此函数
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};