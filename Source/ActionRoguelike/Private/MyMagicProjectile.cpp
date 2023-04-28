// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyAttributeComponent.h"
#include "MyGameplayFunctionLibrary.h"

// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;

}
void AMyMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{        
		////���AttributeComp
		//UMyAttributeComponent* AttributeComp = Cast<UMyAttributeComponent>(OtherActor->GetComponentByClass(UMyAttributeComponent::StaticClass()));
		//        
		////�ٴ��пգ��ص��Ŀ�����û���������������
		//if (AttributeComp) {
		//	AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);
		//	Explode(); 
		//}
		if (UMyGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}

