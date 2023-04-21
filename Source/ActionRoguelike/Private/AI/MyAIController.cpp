// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	//运行行为树
	RunBehaviorTree(BehaviorTree);

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		//设置寻路目标Actor为玩家
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}