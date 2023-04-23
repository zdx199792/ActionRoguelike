// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickup_Health.h"
#include "MyAttributeComponent.h"


AMyPickup_Health::AMyPickup_Health()
{
	
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
		//如果没有满血，则恢复到满血
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			//隐藏生命药水并启动其重生计时器
			HideAndCooldownPickup();
		}	
	}
}