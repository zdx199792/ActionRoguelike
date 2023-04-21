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

	//用于设置传送延迟的时间
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	
	//用于设置引爆延迟的时间
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	AMyDashProjectile();
};
