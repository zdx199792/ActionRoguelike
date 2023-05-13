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
	// 持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	// 周期时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	// 定时器句柄，用于管理两次效果应用之间的时间间隔
	FTimerHandle PeriodHandle;
	// 定时器句柄，用于管理特效的持续时间
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	UMyActionEffect();
};