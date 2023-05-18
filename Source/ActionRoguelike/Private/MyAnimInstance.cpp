// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameplayTagContainer.h"
#include "Actions/MyActionComponent.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// ��ȡ��ǰ����ʵ����������Actor
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		// ���������Actor���ڣ�ͨ��������UMyActionComponent::StaticClass()��ȡActor�е������
		// �����Խ���ת��ΪUMyActionComponent���ͣ�Ȼ��ֵ��ActionComp
		ActionComp = Cast<UMyActionComponent>(OwningActor->GetComponentByClass(UMyActionComponent::StaticClass()));
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// ����һ����Ϊ"Status.Stunned"����Ϸ��ǩ
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		// ���ActionComp���ڣ��ͼ����ActiveGameplayTags�Ƿ����StunnedTag��ǩ�����������ֵ��bIsStunned
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}