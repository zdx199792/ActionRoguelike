// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInteractionInterface.h"
#include "MyPickup.generated.h"

class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API AMyPickup : public AActor, public ISInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPickup();
	void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	
	//用于处理道具的碰撞和交互
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	//用于渲染道具的外观
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	//道具重生的时间间隔
	UPROPERTY(EditAnywhere, Category = "Pickups")
	float RespawnTime;

	//用于管理物品重生的定时器
	FTimerHandle TimerHandle_RespawnTimer;

	//用于显示道具
	UFUNCTION()
	void ShowPickup();
	
	//用于隐藏道具
	void HideAndCooldownPickup();

	//设置道具的激活状态（是否可交互和渲染）
	void SetPickupState(bool bNewIsActive);
};
