// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosionBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplosionBarrel::ASExplosionBarrel()
{

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

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time:%f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
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

