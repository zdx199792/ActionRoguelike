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
    // 如果重叠的物体不是发射该魔法弹道的角色本身
	if (OtherActor && OtherActor != GetInstigator())
	{			
        // 查找该物体是否具有 UMyActionComponent 组件，并且 ActiveGameplayTags 是否包含 ParryTag 标签
		UMyActionComponent* ActionComp = Cast<UMyActionComponent>(OtherActor->GetComponentByClass(UMyActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
            // 如果找到了具有 ParryTag 标签的 UMyActionComponent 组件，反弹该魔法弹道
			MoveComp->Velocity = -MoveComp->Velocity;

            // 设置新的发射者为重叠的物体
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
        // 对重叠的物体施加方向性伤害，如果该物体受到伤害，则执行 Explode() 函数
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

