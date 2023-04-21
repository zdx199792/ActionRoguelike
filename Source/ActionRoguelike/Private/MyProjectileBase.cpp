// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyProjectileBase::AMyProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����������
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//������ײ���collision profile
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &AMyProjectileBase::OnActorHit);
	RootComponent = SphereComp;

	//�����������
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
	MoveComp->bRotationFollowsVelocity = true;			//���ٶȷ��������ת	
	MoveComp->bInitialVelocityInLocalSpace = true;		//���Ƴ�ʼ�ٶ��ڱ�������ռ�(��ɫ)���м���
	MoveComp->ProjectileGravityScale = 0.0f;			//��ʾ���岻������Ӱ��
	MoveComp->InitialSpeed = 8000;						//����������ٶ�

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
}


void AMyProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AMyProjectileBase::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

void AMyProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}