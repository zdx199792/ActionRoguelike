// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MyMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyAttributeComponent.h"
#include "MyGameplayFunctionLibrary.h"
#include "Actions/MyActionComponent.h"
#include "Actions/MyActionEffect.h"
// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyMagicProjectile::OnActorOverlap);
	InitialLifeSpan = 10.0f;
	DamageAmount = 20.0f;

}
void AMyMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // ����ص������岻�Ƿ����ħ�������Ľ�ɫ����
	if (OtherActor && OtherActor != GetInstigator())
	{			
        // ���Ҹ������Ƿ���� UMyActionComponent ��������� ActiveGameplayTags �Ƿ���� ParryTag ��ǩ
		UMyActionComponent* ActionComp = Cast<UMyActionComponent>(OtherActor->GetComponentByClass(UMyActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
            // ����ҵ��˾��� ParryTag ��ǩ�� UMyActionComponent �����������ħ������
			MoveComp->Velocity = -MoveComp->Velocity;

            // �����µķ�����Ϊ�ص�������
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
        // ���ص�������ʩ�ӷ������˺�������������ܵ��˺�����ִ�� Explode() ����
		if (UMyGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
			if (ActionComp && BurningActionClass && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}

