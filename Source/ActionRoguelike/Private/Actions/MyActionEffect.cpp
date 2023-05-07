// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionEffect.h"
#include "Actions/MyActionComponent.h"

UMyActionEffect::UMyActionEffect()
{
	bAutoStart = true;
}

void UMyActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	// 如果持续时间大于0，设置持续时间并在到达时间后停止动作
	if (Duration > 0.0f)
	{
		// 创建一个计时器，计时器结束时调用 StopAction 函数
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	// 如果周期时间大于0，设置周期并在每个周期上执行 ExecutePeriodicEffect() 函数
	if (Period > 0.0f)
	{
		// 创建一个周期性计时器，每周期结束时调用 ExecutePeriodicEffect 函数
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UMyActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// 如果上一个周期已经完成，执行最后一个周期
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	// 父类的停止动作函数
	Super::StopAction_Implementation(Instigator);
	// 清除计时器
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	// 从拥有此动作的组件中删除此动作
	UMyActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UMyActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	// 实现周期性效果的函数，需要在子类中具体实现
}