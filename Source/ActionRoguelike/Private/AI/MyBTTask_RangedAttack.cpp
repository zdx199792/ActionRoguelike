// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取当前任务节点所属的 AI 控制器
	AAIController* MyAIController = OwnerComp.GetAIOwner();
	if (ensure(MyAIController))
	{
		//获取 AI 控制器的控制角色
		ACharacter* MyPawn = Cast<ACharacter>(MyAIController->GetPawn());
		//如果控制角色为空，则返回 EBTNodeResult::Failed 表示任务失败。
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		//获取枪口位置（MuzzleLocation）
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		//获取黑板中保存的目标角色（TargetActor）
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		//如果目标角色为空，则返回 EBTNodeResult::Failed 表示任务失败
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		//计算从枪口位置到目标角色位置的方向向量（Direction），并将其转换为旋转
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		
		//如果新生成的投射物实例不为空，则返回 EBTNodeResult::Succeeded 表示任务成功，否则返回 EBTNodeResult::Failed 表示任务失败
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}	
	return EBTNodeResult::Failed;
}
