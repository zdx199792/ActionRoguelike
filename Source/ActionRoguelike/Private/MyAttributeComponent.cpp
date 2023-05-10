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
	//�������������Ĭ��Replication��Ϊ
	SetIsReplicatedByDefault(true);
}

//����Ƿ���Ѫ
bool UMyAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}
//�����������ֵ
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
	// ��� Delta ֵС�� 0������ʾҪ�Խ�ɫ����˺�
	if (Delta < 0.0f)
	{
		// ��ȡ CVarDamageMultiplier ����̨��������Ϸ�߳��ϵ�ֵ
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		// ����ȫ���˺��������� Delta ֵ
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	//ֻ�ڷ������˽����˺�����
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0.0f)
		{
			//���ʵ�ʱ仯����Ϊ0����֪ͨ�ͻ���
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// ����
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
//�ಥ����������֪ͨ�ͻ�������ֵ�����仯
void UMyAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	//�㲥����ֵ�仯�¼�
	OnHealthChange.Broadcast(InstigatorActor, this, NewHealth, Delta);
}
//�ಥ����������֪ͨ�ͻ��˷�ŭֵ�����仯
void UMyAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
}

//��д����ĺ������������ø�������Ա�����ͬ��
void UMyAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//����Ҫͬ��������
	DOREPLIFETIME(UMyAttributeComponent, Health);
	DOREPLIFETIME(UMyAttributeComponent, MaxHealth);
	DOREPLIFETIME(UMyAttributeComponent, Rage);
	DOREPLIFETIME(UMyAttributeComponent, MaxRage);
	//DOREPLIFETIME_CONDITION(UMyAttributeComponent, MaxHealth, COND_InitialOnly);
}