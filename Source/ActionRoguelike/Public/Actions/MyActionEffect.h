// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "MyActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyActionEffect : public UMyAction
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
		
	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;
protected:
	// ����ʱ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	// ����ʱ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	// ��ʱ����������ڹ�������Ч��Ӧ��֮���ʱ����
	FTimerHandle PeriodHandle;
	// ��ʱ����������ڹ�����Ч�ĳ���ʱ��
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	UMyActionEffect();
};