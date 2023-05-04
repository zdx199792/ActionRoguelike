// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActionComponent.h"
#include "MyAction.h"

// Sets default values for this component's properties
UMyActionComponent::UMyActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyActionComponent::BeginPlay()
{
	Super::BeginPlay();
	// 遍历 DefaultActions 数组，将其中的每个能力添加到 Actions 数组中
	for (TSubclassOf<UMyAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UMyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 添加动作
void UMyActionComponent::AddAction(TSubclassOf<UMyAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	// 创建一个新的动作对象添加到 Actions 数组中
	UMyAction* NewAction = NewObject<UMyAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

//根据名称启动一个动作
bool UMyActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// 遍历 Actions 数组，查找并运行指定名称ActionName的动作
	for (UMyAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

//根据名称停止一个动作
bool UMyActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	// 遍历 Actions 数组，查找并停止指定名称的动作
	for (UMyAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
