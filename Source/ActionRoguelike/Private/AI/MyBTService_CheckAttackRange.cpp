// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UMyBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
        //从黑板组件中获取了名为 "TargetActor" 的黑板键对应的值
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
            //获取了行为树组件的 AI 控制器 AAIController 对象
			AAIController* MyAIController = OwnerComp.GetAIOwner();
			if (ensure(MyAIController)) 
			{
                //获取该 AI 控制器的控制的角色 APawn 对象
				APawn* AIPawn = MyAIController->GetPawn();
				if (ensure(AIPawn))
				{
                    //计算了TargetActor和AI角色的距离
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					//根据距离是否小于1000来设置一个名为 AttackRangeKey 的黑板键对应的布尔值
					bool bWithinRange = DistanceTo < 1000.0f;
					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = MyAIController->LineOfSightTo(TargetActor);
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}