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
	//将 OnPawnSeen 函数与 UPawnSensingComponent 的 OnSeePawn 事件绑定，从而实现了当角色看到其他角色时触发 OnPawnSeen 函数的逻辑
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMyAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChange.AddDynamic(this, &AMyAICharacter::OnHealthChanged);
}

//用于处理当角色看到其他角色时的逻辑
void AMyAICharacter::OnPawnSeen(APawn* Pawn)
{
	//SetTargetActor(Pawn);
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	// 如果当前目标Actor不是已发现的Pawn，则更新目标Actor并创建新的世界Widget
	if (GetTargetActor() != Pawn)
	{
		//更新目标Actor
		SetTargetActor(Pawn);
		//创建新的发现提示Widget
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
		// 如果血条 UI 没有被创建，则创建血条 UI
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UMyWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				// 将血条 UI 关联到当前角色
				ActiveHealthBar->AttachedActor = this;
				// 将血条 UI 添加到视图中
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		//AI角色死亡
		if (NewHealth <= 0.0f)
		{
			//停止行为树
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
            //设置模拟物理效果
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			//设置尸体存留时间
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
