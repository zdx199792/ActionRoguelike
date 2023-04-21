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
        //�Ӻڰ�����л�ȡ����Ϊ "TargetActor" �ĺڰ����Ӧ��ֵ
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
            //��ȡ����Ϊ������� AI ������ AAIController ����
			AAIController* MyAIController = OwnerComp.GetAIOwner();
			if (ensure(MyAIController)) 
			{
                //��ȡ�� AI �������Ŀ��ƵĽ�ɫ APawn ����
				APawn* AIPawn = MyAIController->GetPawn();
				if (ensure(AIPawn))
				{
                    //������TargetActor��AI��ɫ�ľ���
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					//���ݾ����Ƿ�С��1000������һ����Ϊ AttackRangeKey �ĺڰ����Ӧ�Ĳ���ֵ
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