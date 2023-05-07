// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "MyAttributeComponent.h"
#include "BrainComponent.h"
#include "MyWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyActionComponent.h"

// Sets default values
AMyAICharacter::AMyAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UMyAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UMyActionComponent>("ActionComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
	TimeToHitParamName = "TimeToHit";
}

void AMyAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//�� OnPawnSeen ������ UPawnSensingComponent �� OnSeePawn �¼��󶨣��Ӷ�ʵ���˵���ɫ����������ɫʱ���� OnPawnSeen �������߼�
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMyAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChange.AddDynamic(this, &AMyAICharacter::OnHealthChanged);
}

//���ڴ�����ɫ����������ɫʱ���߼�
void AMyAICharacter::OnPawnSeen(APawn* Pawn)
{
	//SetTargetActor(Pawn);
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	// �����ǰĿ��Actor�����ѷ��ֵ�Pawn�������Ŀ��Actor�������µ�����Widget
	if (GetTargetActor() != Pawn)
	{
		//����Ŀ��Actor
		SetTargetActor(Pawn);
		//�����µķ�����ʾWidget
		UMyWorldUserWidget* NewWidget = CreateWidget<UMyWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			NewWidget->AddToViewport(10);
		}
	}
}

void AMyAICharacter::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		// ���Ѫ�� UI û�б��������򴴽�Ѫ�� UI
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UMyWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				// ��Ѫ�� UI ��������ǰ��ɫ
				ActiveHealthBar->AttachedActor = this;
				// ��Ѫ�� UI ��ӵ���ͼ��
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		//AI��ɫ����
		if (NewHealth <= 0.0f)
		{
			//ֹͣ��Ϊ��
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
            //����ģ������Ч��
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			//����ʬ�����ʱ��
			SetLifeSpan(10.0f);
		}
	}
}
void AMyAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}
AActor* AMyAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}
