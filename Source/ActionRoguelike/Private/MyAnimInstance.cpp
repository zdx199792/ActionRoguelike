// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameplayTagContainer.h"
#include "Actions/MyActionComponent.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 获取当前动画实例的所有者Actor
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		// 如果所有者Actor存在，通过类类型UMyActionComponent::StaticClass()获取Actor中的组件，
		// 并尝试将其转化为UMyActionComponent类型，然后赋值给ActionComp
		ActionComp = Cast<UMyActionComponent>(OwningActor->GetComponentByClass(UMyActionComponent::StaticClass()));
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 请求一个名为"Status.Stunned"的游戏标签
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		// 如果ActionComp存在，就检查其ActiveGameplayTags是否包含StunnedTag标签，并将结果赋值给bIsStunned
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}