// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MyPickup_Health.h"
#include "MyAttributeComponent.h"
#include "MyPlayerState.h"
// 定义一个本地化文本的命名空间为"InteractableActors"
#define LOCTEXT_NAMESPACE "InteractableActors"
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

FText AMyPickup_Health::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// 如果健康值已满
	UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		// 返回一段本地化文本，文本的key为"HealthPotion_FullHealthWarning"，默认值为"Already at full health."
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}
	// 返回一段格式化的本地化文本，文本的key为"HealthPotion_InteractMessage"，
	// 默认值为"Cost {0} Credits. Restores health to maximum."，并用CreditCost替换掉文本中的{0}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);}
// 取消之前定义的本地化文本的命名空间
#undef LOCTEXT_NAMESPACE