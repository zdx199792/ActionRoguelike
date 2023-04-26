// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_HealSelf.h"
#include "MyAttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UMyBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}
