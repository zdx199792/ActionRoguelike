// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MyPickup_Action.h"
#include "Actions/MyActionComponent.h"
#include "Actions/MyAction.h"

void AMyPickup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
    // 确保InstigatorPawn和ActionToGrant都存在
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	// 获取InstigatorPawn身上的UMyActionComponent组件
	UMyActionComponent* ActionComp = Cast<UMyActionComponent>(InstigatorPawn->GetComponentByClass(UMyActionComponent::StaticClass()));
	if (ActionComp)
	{
        // 如果该ActionToGrant已存在，则返回
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}
		// 否则添加该ActionToGrant到ActionComp中
		ActionComp->AddAction(InstigatorPawn, ActionToGrant); 
         // 隐藏并冷却生成该物品
		HideAndCooldownPickup();
	}
}