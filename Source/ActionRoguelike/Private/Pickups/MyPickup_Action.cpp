// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MyPickup_Action.h"
#include "Actions/MyActionComponent.h"
#include "Actions/MyAction.h"

void AMyPickup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
    // ȷ��InstigatorPawn��ActionToGrant������
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	// ��ȡInstigatorPawn���ϵ�UMyActionComponent���
	UMyActionComponent* ActionComp = Cast<UMyActionComponent>(InstigatorPawn->GetComponentByClass(UMyActionComponent::StaticClass()));
	if (ActionComp)
	{
        // �����ActionToGrant�Ѵ��ڣ��򷵻�
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}
		// ������Ӹ�ActionToGrant��ActionComp��
		ActionComp->AddAction(InstigatorPawn, ActionToGrant); 
         // ���ز���ȴ���ɸ���Ʒ
		HideAndCooldownPickup();
	}
}