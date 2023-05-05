// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	//ָ��Ҫ���ɵ�AI��ɫ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

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

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* PickupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float RequiredPickupDistance;

	/* Amount of powerups to spawn during match start */
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

public:
	AMyGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};