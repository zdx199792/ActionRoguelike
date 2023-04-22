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

//���ڴ�����ɫ����������ɫʱ���߼�
void AMyAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	//��AI������ɫ����ʱ������"TargetActor" �ĺڰ����ֵΪ��⵽�Ľ�ɫ���� Pawn��
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
	//�� OnPawnSeen ������ UPawnSensingComponent �� OnSeePawn �¼��󶨣��Ӷ�ʵ���˵���ɫ����������ɫʱ���� OnPawnSeen �������߼�
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMyAICharacter::OnPawnSeen);
}
