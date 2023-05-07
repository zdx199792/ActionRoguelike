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
	// �������ʱ�����0�����ó���ʱ�䲢�ڵ���ʱ���ֹͣ����
	if (Duration > 0.0f)
	{
		// ����һ����ʱ������ʱ������ʱ���� StopAction ����
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	// �������ʱ�����0���������ڲ���ÿ��������ִ�� ExecutePeriodicEffect() ����
	if (Period > 0.0f)
	{
		// ����һ�������Լ�ʱ����ÿ���ڽ���ʱ���� ExecutePeriodicEffect ����
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UMyActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// �����һ�������Ѿ���ɣ�ִ�����һ������
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	// �����ֹͣ��������
	Super::StopAction_Implementation(Instigator);
	// �����ʱ��
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	// ��ӵ�д˶����������ɾ���˶���
	UMyActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UMyActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	// ʵ��������Ч���ĺ�������Ҫ�������о���ʵ��
}