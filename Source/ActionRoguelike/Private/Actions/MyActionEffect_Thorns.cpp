// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionEffect_Thorns.h"
#include "MyAttributeComponent.h"
#include "Actions/MyActionComponent.h"
#include "MyGameplayFunctionLibrary.h"


// ���캯��������Ĭ�Ϸ����˺�����������ʱ��ͼ��ʱ��
UMyActionEffect_Thorns::UMyActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	Duration = 0.0f;
	Period = 0.0f;
}
// ����Ч����ʼ�ķ���
void UMyActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UMyAttributeComponent* Attributes = UMyAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		// �� OnHealthChange �¼�������,�¼�����ʱִ��OnHealthChanged����
		Attributes->OnHealthChange.AddDynamic(this, &UMyActionEffect_Thorns::OnHealthChanged);
	}
}
// ����Ч�������ķ���
void UMyActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UMyAttributeComponent* Attributes = UMyAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChange.RemoveDynamic(this, &UMyActionEffect_Thorns::OnHealthChanged);
	}
}
// �����ɫ�ܵ��˺�ʱ���¼�
void UMyActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// ��ȡ���з���Ч���Ľ�ɫ
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// ����˺�ֵС�� 0�����˺���Դ���ǳ��и�Ч���Ľ�ɫ�Լ������з����˺�
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// ���㷴���˺�ֵ
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}
		ReflectedAmount = FMath::Abs(ReflectedAmount);
		// Ӧ���˺�
		UMyGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}