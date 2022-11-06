// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosionBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"

// Sets default values
ASExplosionBarrel::ASExplosionBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建静态网格体
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//开启"模拟物理"
	MeshComp->SetSimulatePhysics(true);
	//设置碰撞预设为PhysicsActor
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = MeshComp;

	//创建爆炸作用力
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->Radius = 500.0f;				//作用力范围
	ForceComp->ImpulseStrength = 500.0f;	//作用力大小
	ForceComp->bImpulseVelChange = true;	//作用力效果忽略质量大小

	//注册组件的碰撞事件，绑定到OnActorHit函数上
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosionBarrel::OnActorHit);

}

void ASExplosionBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	//ForceComp释放作用力
	ForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void ASExplosionBarrel::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASExplosionBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

