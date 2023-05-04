// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UMyAction_ProjectileAttack::UMyAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.5f;
}

//ʵ�ֹ����������߼�
void UMyAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	// ��� Instigator �� ACharacter ���ͣ�����й���
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		//���Ź�������
		Character->PlayAnimMontage(AttackAnim);
		// ���ֲ� socket �ϲ���������Ч
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		// ����һ����ʱ�����ڹ��������ӳ�֮����� AttackDelay_Elapsed ����
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

//�ڹ��������ӳ�֮����ã����ɵ��������� Projectile ����
void UMyAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		// ��ȡ�ֲ� socket ��λ��
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		// �������� Projectile ����Ĳ���
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		// ������ײ��״
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		// ������ײ��ѯ����
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		// ������ײ�����ѯ����
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// ��ȡ TraceStart �� TraceEnd�����ڽ�����ײ���
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);
		// ������ײ��⣬���������ײ���� TraceEnd ����Ϊ��ײ��
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// ���� TraceEnd �� HandLocation �������� Projectile �������ת
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		// �������� Projectile �����λ�ú���ת
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// ���� Projectile ����
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	// ֹͣ����
	StopAction(InstigatorCharacter);
}