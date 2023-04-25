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

	//��ʱ���ص���������������AI��ɫ
	UFUNCTION()
	void SpawnBotTimerElapsed();

	//EQS��ѯ��ɵĻص����������ڴ����ѯ���
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:

	AMyGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
};