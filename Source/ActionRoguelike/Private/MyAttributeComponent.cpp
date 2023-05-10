// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeComponent.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "../ActionRoguelike.h"
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UMyAttributeComponent::UMyAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;

	MaxRage = 100;
	Rage = 0;
	//用于设置组件的默认Replication行为
	SetIsReplicatedByDefault(true);
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	//只在服务器端进行伤害计算
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0.0f)
		{
			//如果实际变化量不为0，则通知客户端
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// 死亡
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	return ActualDelta != 0;
}
bool UMyAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	float NewRage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);
	float ActualDelta = NewRage - OldRage;
	if (GetOwner()->HasAuthority())
	{
		Rage = NewRage;
		if (ActualDelta != 0.0f) 
		{
			//OnRageChange.Broadcast(InstigatorActor,this, Rage,ActualDelta);
			MulticastRageChanged(InstigatorActor, Rage, ActualDelta);

		}
	}
	return ActualDelta != 0;
}
float UMyAttributeComponent::GetRage() const
{
	return Rage;
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
//多播函数，用于通知客户端生命值发生变化
void UMyAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	//广播生命值变化事件
	OnHealthChange.Broadcast(InstigatorActor, this, NewHealth, Delta);
}
//多播函数，用于通知客户端愤怒值发生变化
void UMyAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
}

//重写父类的函数，用于设置该组件可以被网络同步
void UMyAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//声明要同步的属性
	DOREPLIFETIME(UMyAttributeComponent, Health);
	DOREPLIFETIME(UMyAttributeComponent, MaxHealth);
	DOREPLIFETIME(UMyAttributeComponent, Rage);
	DOREPLIFETIME(UMyAttributeComponent, MaxRage);
	//DOREPLIFETIME_CONDITION(UMyAttributeComponent, MaxHealth, COND_InitialOnly);
}