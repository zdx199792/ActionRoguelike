// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionEffect.h"
#include "Actions/MyActionComponent.h"
#include "GameFramework/GameStateBase.h"

UMyActionEffect::UMyActionEffect()
{
	bAutoStart = true;
}
float UMyActionEffect::GetTimeRemaining() const
{
	// 获取当前世界的游戏状态
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		// 计算动作结束的时间（动作开始的时间加上动作的持续时间）
		float EndTime = TimeStarted + Duration;
		// 返回动作结束时间和当前服务器时间的差值，这就是动作的剩余时间
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	// 如果没有有效的游戏状态，那么就返回动作的持续时间，作为动作的剩余时间
	return Duration;
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