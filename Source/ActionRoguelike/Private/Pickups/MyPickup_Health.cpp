// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MyPickup_Health.h"
#include "MyAttributeComponent.h"
#include "MyPlayerState.h"

AMyPickup_Health::AMyPickup_Health()
{
	CreditCost = 50;
}


void AMyPickup_Health::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(InstigatorPawn->GetComponentByClass(UMyAttributeComponent::StaticClass()));
	// 玩家是否满血
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AMyPlayerState* PS = InstigatorPawn->GetPlayerState<AMyPlayerState>())
		{
			// 如果玩家的 Credits 足够支付 Pickup_Health 的花费，则启用 Pickup，并扣除 Credits
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPickup();
			}
		}	
	}
}