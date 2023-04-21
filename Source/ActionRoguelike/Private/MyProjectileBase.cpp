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

	//创建球体组件
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//设置碰撞体的collision profile
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &AMyProjectileBase::OnActorHit);
	RootComponent = SphereComp;

	//创建抛体组件
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
	MoveComp->bRotationFollowsVelocity = true;			//随速度方向更新旋转	
	MoveComp->bInitialVelocityInLocalSpace = true;		//控制初始速度在本地坐标空间(角色)进行计算
	MoveComp->ProjectileGravityScale = 0.0f;			//表示抛体不受重力影响
	MoveComp->InitialSpeed = 8000;						//抛体组件初速度

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