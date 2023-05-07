// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionEffect_Thorns.h"
#include "MyAttributeComponent.h"
#include "Actions/MyActionComponent.h"
#include "MyGameplayFunctionLibrary.h"


// 构造函数，设置默认反弹伤害比例、持续时间和间隔时间
UMyActionEffect_Thorns::UMyActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	Duration = 0.0f;
	Period = 0.0f;
}
// 反伤效果开始的方法
void UMyActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UMyAttributeComponent* Attributes = UMyAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		// 绑定 OnHealthChange 事件处理方法,事件触发时执行OnHealthChanged方法
		Attributes->OnHealthChange.AddDynamic(this, &UMyActionEffect_Thorns::OnHealthChanged);
	}
}
// 反伤效果结束的方法
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
// 处理角色受到伤害时的事件
void UMyActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 获取持有反伤效果的角色
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// 如果伤害值小于 0，且伤害来源不是持有该效果的角色自己，进行反弹伤害
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// 计算反弹伤害值
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}
		ReflectedAmount = FMath::Abs(ReflectedAmount);
		// 应用伤害
		UMyGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}