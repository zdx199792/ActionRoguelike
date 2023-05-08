// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyAction.h"
#include "Actions/MyActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UMyAction::Initialize(UMyActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void UMyAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);	
	// 将GrantsTags添加到拥有UMyAction实例的角色中
	UMyActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	// 将bIsRunning设置为true，表示UMyAction正在运行
	bIsRunning = true;
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	/*UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));*/
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	// 从拥有UMyAction实例的角色中移除GrantsTags
	UMyActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	// 将bIsRunning设置为false，表示UMyAction已停止运行
	bIsRunning = false;
}
//用于检查UMyAction是否可以开始。如果UMyAction正在运行或拥有UMyAction实例的角色有被阻止启动的标签，则返回false，否则返回true。
bool UMyAction::CanStart_Implementation(AActor* Instigator)
{
	// 如果UMyAction正在运行，返回false（正在攻击时不能再次攻击）
	if (IsRunning())
	{
		return false;
	}

	UMyActionComponent* Comp = GetOwningComponent();
	// 如果拥有UMyAction实例的角色有任何一个BlockedTags，则返回false
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	// 如果UMyAction不在运行状态且拥有UMyAction实例的角色没有BlockedTags，则返回true
	return true;
}
//用于获取UMyAction实例所在的世界，通过查询其所在的容器对象来实现
UWorld* UMyAction::GetWorld() const
{	
    // 获取该Action所在的容器对象
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
        // 如果容器对象为 AActor 类型，返回其所在的世界的指针
		return Actor->GetWorld();
	}
    // 如果无法获取到容器对象或者世界的指针，则返回 nullptr
	return nullptr;
}
//在UMyAction实例中可以通过该函数获取其所属的UMyActionComponent实例
UMyActionComponent* UMyAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(UMyActionComponent::StaticClass());
	return ActionComp;
}

bool UMyAction::IsRunning() const
{
	return bIsRunning;
}
//希望服务器能够告诉所有的客户端
void UMyAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void UMyAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyAction, bIsRunning);
	DOREPLIFETIME(UMyAction, ActionComp);
}