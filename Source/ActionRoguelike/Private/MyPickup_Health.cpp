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
	// ����Ƿ���Ѫ
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		//���û����Ѫ����ָ�����Ѫ
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			//��������ҩˮ��������������ʱ��
			HideAndCooldownPickup();
		}	
	}
}