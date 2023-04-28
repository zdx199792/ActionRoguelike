// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeComponent.h"
#include "MyGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UMyAttributeComponent::UMyAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}
//玩家是否满血
bool UMyAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}
//返回最大生命值
float UMyAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}
bool UMyAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	// 如果 Delta 值小于 0，即表示要对角色造成伤害
	if (Delta < 0.0f)
	{
		// 获取 CVarDamageMultiplier 控制台变量在游戏线程上的值
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		// 根据全局伤害乘数调整 Delta 值
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	//将 Health 的值限制在 0 到 MaxHealth 之间
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	//计算实际的生命值变化量
	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(InstigatorActor,this, Health,ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool UMyAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
UMyAttributeComponent* UMyAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UMyAttributeComponent>(FromActor->GetComponentByClass(UMyAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UMyAttributeComponent::IsActorAlive(AActor* Actor)
{
	UMyAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}
bool UMyAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

float UMyAttributeComponent::GetHealth() const
{
	return Health;
}