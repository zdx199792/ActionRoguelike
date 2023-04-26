// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAttributeComponent.h"
#include "AIController.h"

UMyBTService_CheckHealth::UMyBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}
void UMyBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ensure(MyPawn))
	{
		UMyAttributeComponent* AIAttributeComp = UMyAttributeComponent::GetAttributes(MyPawn);
		if (AIAttributeComp)
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			bool bLowHealth = AIAttributeComp->GetHealth() / AIAttributeComp->GetHealthMax() < LowHealthFraction;
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
