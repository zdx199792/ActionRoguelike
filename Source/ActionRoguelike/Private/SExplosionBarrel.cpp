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
	//ForceComp�ͷ�������
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

