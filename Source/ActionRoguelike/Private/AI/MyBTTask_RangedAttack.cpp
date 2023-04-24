// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAttributeComponent.h"

UMyBTTask_RangedAttack::UMyBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type UMyBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//��ȡ��ǰ����ڵ������� AI ������
	AAIController* MyAIController = OwnerComp.GetAIOwner();
	if (ensure(MyAIController))
	{
		//��ȡ AI �������Ŀ��ƽ�ɫ
		ACharacter* MyPawn = Cast<ACharacter>(MyAIController->GetPawn());
		//������ƽ�ɫΪ�գ��򷵻� EBTNodeResult::Failed ��ʾ����ʧ�ܡ�
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		//��ȡǹ��λ�ã�MuzzleLocation��
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		//��ȡ�ڰ��б����Ŀ���ɫ��TargetActor��
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		//���Ŀ���ɫΪ�գ��򷵻� EBTNodeResult::Failed ��ʾ����ʧ��
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		if (!UMyAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		//�����ǹ��λ�õ�Ŀ���ɫλ�õķ���������Direction����������ת��Ϊ��ת
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		
		//��������ɵ�Ͷ����ʵ����Ϊ�գ��򷵻� EBTNodeResult::Succeeded ��ʾ����ɹ������򷵻� EBTNodeResult::Failed ��ʾ����ʧ��
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}	
	return EBTNodeResult::Failed;
}
