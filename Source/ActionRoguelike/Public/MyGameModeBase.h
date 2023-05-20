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

// ����һ���ṹ��FMonsterInfoRow���̳���FTableRowBase����һ����ʾ������Ϣ�����ݱ�
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Ĭ�Ϲ��캯��
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	// ����һ����ΪMonsterId���ֶΣ�����ΪFPrimaryAssetId�����ֶ����ڱ�ʶ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	// ����һ����ΪWeight���ֶΣ�����Ϊfloat�����ֶα�ʾѡ��˹������Ի���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
	// ����һ����ΪSpawnCost���ֶΣ�����Ϊfloat�����ֶα�ʾ��Ϸģʽ���ɴ˵�λ����ĵ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	// ����һ����ΪKillReward���ֶΣ�����Ϊfloat�����ֶα�ʾɱ���˵�λ�Ľ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// ���ڱ�����Ϸ�浵�Ĳ�λ����
	FString SlotName;
	// ��ǰ����Ϸ�浵����
	UPROPERTY()
	UMySaveGame* CurrentSaveGame;


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	//ָ��Ҫ���ɵ�AI��ɫ����
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;

	//ָ����������AI��ɫ�Ļ�����ѯϵͳ��EQS����ѯ
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	//ָ����Ϸ���Ѷ�����(AI��ɫ��������)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	//����ʱ����AI��ɫ�ļ�ʱ��
	FTimerHandle TimerHandle_SpawnBots;

	//ָ������AI��ɫ��ʱ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	//ɱ��AI�����˵ĵ÷ֽ���ֵ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	//����ʰȡ���EQS��ѯ
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* PickupSpawnQuery;

	//ʰȡ��������
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	//ʰȡ��֮��ļ��
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float RequiredPickupDistance;

	//ʰȡ����������
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 DesiredPickupCount;

	//��ʱ���ص���������������AI��ɫ
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
	// ��дAGameModeBase��InitGame������������Ϸ��ʼ��
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// ����Ϸ����д��浵
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	// ������Ϸ�浵�ĺ���
	void LoadSaveGame();
	//��������Ҽ�����Ϸ���߼�������������ӵ���Ϸ������ʱ������������ô˺���
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};