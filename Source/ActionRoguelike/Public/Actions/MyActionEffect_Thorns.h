// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActionEffect.h"
#include "MyActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyActionEffect_Thorns : public UMyActionEffect
{
	GENERATED_BODY()
protected:
	// 反弹伤害系数
	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;
	// 生命值变化时调用的回调函数，用于监听生命值变化
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta);
public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
	UMyActionEffect_Thorns();
};
