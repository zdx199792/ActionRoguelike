// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionComponent.h"
#include "Actions/MyAction.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
// Sets default values for this component's properties
UMyActionComponent::UMyActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UMyActionComponent::BeginPlay()
{
	Super::BeginPlay();
	// 检查当前对象的拥有者是否具有网络权限(服务器)
	if (GetOwner()->HasAuthority())
	{
		// 如果具有网络权限(是服务器)，遍历 DefaultActions 数组，将其中的每个能力添加到 Actions 数组中
		for (TSubclassOf<UMyAction> ActionClass : DefaultActions)
		{
			// 将ActionClass实例添加到当前对象的Actions数组中
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UMyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	
	// 遍历Actions数组中的每一个UMyAction对象
	for (UMyAction* Action : Actions)
	{
		// 根据UMyAction对象是否正在运行，设置文本颜色为蓝色或白色
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

	    // 使用格式化字符串（Printf）创建一个包含UMyAction对象信息的消息
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),							// GetOwner()的安全名称
			*Action->ActionName.ToString(),						// Action的名称
			Action->IsRunning() ? TEXT("true") : TEXT("false"), // Action是否正在运行（true或false）
			*GetNameSafe(Action->GetOuter()));					// Action->GetOuter()的安全名称
		// 调用之前定义的LogOnScreen函数，将ActionMsg显示在屏幕上，颜色为TextColor，持续时间为0秒
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

// 添加动作
void UMyActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMyAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	// 创建一个新的动作对象添加到 Actions 数组中
	UMyAction* NewAction = NewObject<UMyAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		// 检查新添加的动作是否需要自动启动，如果需要，并且满足启动条件，就调用 StartAction 函数来启动该动作
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}
void UMyActionComponent::RemoveAction(UMyAction* ActionToRemove)
{
	// 检查 ActionToRemove 是否存在且未运行
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	// 从 Actions 数组中移除 ActionToRemove
	Actions.Remove(ActionToRemove);
}

UMyAction* UMyActionComponent::GetAction(TSubclassOf<UMyAction> ActionClass) const
{
	for (UMyAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}
//根据名称启动一个动作
bool UMyActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// 遍历 Actions 数组，查找并运行指定名称ActionName的动作
	for (UMyAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			// 检查该动作是否可以启动
			if (!Action->CanStart(Instigator))
			{
				// 如果无法启动，输出错误信息，并继续查找下一个动作
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			// 如果当前组件不在服务器上，向服务器发送 RPC 启动动作
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}
// 实现服务器 RPC 函数 ServerStartAction，调用 StartActionByName 启动动作
void UMyActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}
/*ReplicateSubobjects是一个虚函数，用于在Unreal Engine中处理子对象的网络复制。
当有一个继承自AActor或UActorComponent的类，并且该类具有需要在网络中进行复制的子对象时，
需要重载ReplicateSubobjects函数。这个函数用于定义如何复制子对象以保证服务器和客户端之间的游戏状态同步。*/
bool UMyActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	// 遍历Actions数组中的每一个UMyAction对象
	for (UMyAction* Action : Actions)
	{
		if (Action)
		{
			// 调用Channel->ReplicateSubobject方法为当前Action对象进行网络复制
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	// 返回是否成功复制了任何子对象
	return WroteSomething;
}
//复制属性
void UMyActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyActionComponent, Actions);
}