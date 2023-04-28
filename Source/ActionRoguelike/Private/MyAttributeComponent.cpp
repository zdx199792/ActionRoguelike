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
	//�� Health ��ֵ������ 0 �� MaxHealth ֮��
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	//����ʵ�ʵ�����ֵ�仯��
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