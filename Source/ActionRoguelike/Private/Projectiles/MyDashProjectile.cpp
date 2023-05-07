// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MyDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMyDashProjectile::AMyDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f;
}
//ʵ������Ч��
void AMyDashProjectile::Explode_Implementation()
{
	//�����֮ǰ���õĶ�ʱ��TimerHandle_DelayedDetonate
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	//�ڵ�ǰλ����������Ч��
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	
	//����EffectComp->DeactivateSystem()����ֹͣ����Ч���Ĳ���
	EffectComp->DeactivateSystem();
	
	//����MoveComp->StopMovementImmediately()����ֹͣ�ƶ�
	MoveComp->StopMovementImmediately();

	//����SetActorEnableCollision(false) ������������ײ
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	//����GetWorldTimerManager().SetTimer()������һ���ӳ�ִ�еĶ�ʱ��TimerHandle_DelayedTeleport:
	//��ָ�����ӳ�ʱ��������AMyDashProjectile::TeleportInstigator()����
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AMyDashProjectile::TeleportInstigator, TeleportDelay);
}

//��ʱ������ʱ��ִ�еĴ����߼�
void AMyDashProjectile::TeleportInstigator()
{
	//��ȡ��Ͷ�����Instigator�������ߣ�
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//��һ����Ϸ��ɫ������˲�䴫�͵�ָ��λ��
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

void AMyDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	//����һ���ӳ�ִ�еĶ�ʱ��TimerHandle_DelayedDetonate����ָ�����ӳ�ʱ��������AMyDashProjectile::Explode_Implementation()�������Ӷ���������Ч��
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AMyDashProjectile::Explode_Implementation, DetonateDelay);
}
