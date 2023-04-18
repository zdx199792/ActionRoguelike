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
	//������ײ���collision profile
	SphereComp->SetCollisionProfileName("Projectile");
	//ħ��������Actor�ص�ʱ�����˺�����
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyMagicProjectile::OnActorOverlap);

	RootComponent = SphereComp;

	//�����������
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;				//����������ٶ�
	MovementComp->bRotationFollowsVelocity = true;		//���ٶȷ��������ת
	MovementComp->bInitialVelocityInLocalSpace = true;

	//��������ϵͳ���
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
}
void AMyMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{        
		//���AttributeComp
		UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(OtherActor->GetComponentByClass(UMyAttributeComponent::StaticClass()));
		        
		//�ٴ��пգ��ص��Ŀ�����û���������������
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

