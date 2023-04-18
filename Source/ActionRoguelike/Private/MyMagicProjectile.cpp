// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyAttributeComponent.h"

// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//设置碰撞体的collision profile
	SphereComp->SetCollisionProfileName("Projectile");
	//魔法攻击与Actor重叠时触发伤害计算
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyMagicProjectile::OnActorOverlap);

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
void AMyMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{        
		//获得AttributeComp
		UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(OtherActor->GetComponentByClass(UMyAttributeComponent::StaticClass()));
		        
		//再次判空，重叠的可能是没有属性组件的物体
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(-20.0f);
			Destroy(); 
		}
	}
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

