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
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);	
	// 将GrantsTags添加到拥有UMyAction实例的角色中
	UMyActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	// 将bIsRunning设置为true，表示UMyAction正在运行
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	// 如果当前动作所属的组件的拥有者处于服务器角色（ROLE_Authority）
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		// 记录当前世界时间作为动作开始的时间
		TimeStarted = GetWorld()->TimeSeconds;
	}
	// 当动作开始时，广播OnActionStarted事件
	// GetOwningComponent()获取当前动作所属的组件，this代表当前动作实例
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	/*UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));*/
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	// 从拥有UMyAction实例的角色中移除GrantsTags
	UMyActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	// 将bIsRunning设置为false，表示UMyAction已停止运行
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	// 当动作停止时，广播OnActionStopped事件
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
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
	return RepData.bIsRunning;
}
//被服务器告知RepData属性变化后的回调函数，根据RepData.bIsRunning来更新是否执行该MyAction动作
void UMyAction::OnRep_RepData()
{    
	//如果RepData.bIsRunning为真，则会调用该MyAction的StartAction从而更新本地客户端这个MyAction的动作
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UMyAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyAction, RepData);
	DOREPLIFETIME(UMyAction, ActionComp);
	DOREPLIFETIME(UMyAction, TimeStarted);
}