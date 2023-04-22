// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyAICharacter::AMyAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

//用于处理当角色看到其他角色时的逻辑
void AMyAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	//当AI看到角色对象时，设置"TargetActor" 的黑板键的值为检测到的角色对象 Pawn。
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

void AMyAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//将 OnPawnSeen 函数与 UPawnSensingComponent 的 OnSeePawn 事件绑定，从而实现了当角色看到其他角色时触发 OnPawnSeen 函数的逻辑
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMyAICharacter::OnPawnSeen);
}
