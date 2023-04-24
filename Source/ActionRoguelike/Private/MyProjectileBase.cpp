// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AMyProjectileBase::AMyProjectileBase()
{
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

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;
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
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		Destroy();
	}
}

void AMyProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}