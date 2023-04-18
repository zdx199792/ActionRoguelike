// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosionBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplosionBarrel::ASExplosionBarrel()
{

	//������̬������
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//����"ģ������"
	MeshComp->SetSimulatePhysics(true);
	//������ײԤ��ΪPhysicsActor
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = MeshComp;

	//������ը������
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->Radius = 500.0f;				//��������Χ
	ForceComp->ImpulseStrength = 500.0f;	//��������С
	ForceComp->bImpulseVelChange = true;	//������Ч������������С

	//ע���������ײ�¼����󶨵�OnActorHit������
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

