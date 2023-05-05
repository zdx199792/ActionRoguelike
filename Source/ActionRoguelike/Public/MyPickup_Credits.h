// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPickup.h"
#include "MyPickup_Credits.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyPickup_Credits : public AMyPickup
{
	GENERATED_BODY()

protected:
	//Pickup 为玩家增加的 Credits 值
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	AMyPickup_Credits();
};
