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
	// �����˺�ϵ��
	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;
	// ����ֵ�仯ʱ���õĻص����������ڼ�������ֵ�仯
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta);
public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
	UMyActionEffect_Thorns();
};
