// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPickup.h"
#include "MyPickup_Action.generated.h"
class UMyAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyPickup_Action : public AMyPickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Pickups")
	TSubclassOf<UMyAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
