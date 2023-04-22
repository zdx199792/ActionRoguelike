// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	//������Ϊ��
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr, Please assign Behavior Tree in your AIController")))
	{	
		RunBehaviorTree(BehaviorTree);
	}
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	//����Ѱ·Ŀ��ActorΪ���
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}