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
	// ����Ƿ���Ѫ
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AMyPlayerState* PS = InstigatorPawn->GetPlayerState<AMyPlayerState>())
		{
			// �����ҵ� Credits �㹻֧�� Pickup_Health �Ļ��ѣ������� Pickup�����۳� Credits
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPickup();
			}
		}	
	}
}