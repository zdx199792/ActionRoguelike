// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectileBase.h"
#include "MyDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyDashProjectile : public AMyProjectileBase
{
	GENERATED_BODY()

protected:

	//�������ô����ӳٵ�ʱ��
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	
	//�������������ӳٵ�ʱ��
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	AMyDashProjectile();
};
