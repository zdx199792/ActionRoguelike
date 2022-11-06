// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//设置碰撞体的collision profile
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
	
    //创建抛体组件
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;				//抛体组件初速度
	MovementComp->bRotationFollowsVelocity = true;		//随速度方向更新旋转
	MovementComp->bInitialVelocityInLocalSpace = true;	
	
    //创建粒子系统组件
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void AMyMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

