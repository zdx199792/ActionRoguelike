// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MyPickup_Health.h"
#include "MyAttributeComponent.h"
#include "MyPlayerState.h"
// ����һ�����ػ��ı��������ռ�Ϊ"InteractableActors"
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

FText AMyPickup_Health::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// �������ֵ����
	UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		// ����һ�α��ػ��ı����ı���keyΪ"HealthPotion_FullHealthWarning"��Ĭ��ֵΪ"Already at full health."
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}
	// ����һ�θ�ʽ���ı��ػ��ı����ı���keyΪ"HealthPotion_InteractMessage"��
	// Ĭ��ֵΪ"Cost {0} Credits. Restores health to maximum."������CreditCost�滻���ı��е�{0}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);}
// ȡ��֮ǰ����ı��ػ��ı��������ռ�
#undef LOCTEXT_NAMESPACE