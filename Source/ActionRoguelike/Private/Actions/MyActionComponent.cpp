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
	// ��鵱ǰ�����ӵ�����Ƿ��������Ȩ��(������)
	if (GetOwner()->HasAuthority())
	{
		// �����������Ȩ��(�Ƿ�����)������ DefaultActions ���飬�����е�ÿ��������ӵ� Actions ������
		for (TSubclassOf<UMyAction> ActionClass : DefaultActions)
		{
			// ��ActionClassʵ����ӵ���ǰ�����Actions������
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UMyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	
	// ����Actions�����е�ÿһ��UMyAction����
	for (UMyAction* Action : Actions)
	{
		// ����UMyAction�����Ƿ��������У������ı���ɫΪ��ɫ���ɫ
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

	    // ʹ�ø�ʽ���ַ�����Printf������һ������UMyAction������Ϣ����Ϣ
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),							// GetOwner()�İ�ȫ����
			*Action->ActionName.ToString(),						// Action������
			Action->IsRunning() ? TEXT("true") : TEXT("false"), // Action�Ƿ��������У�true��false��
			*GetNameSafe(Action->GetOuter()));					// Action->GetOuter()�İ�ȫ����
		// ����֮ǰ�����LogOnScreen��������ActionMsg��ʾ����Ļ�ϣ���ɫΪTextColor������ʱ��Ϊ0��
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

// ��Ӷ���
void UMyActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMyAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	// ����һ���µĶ���������ӵ� Actions ������
	UMyAction* NewAction = NewObject<UMyAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		// �������ӵĶ����Ƿ���Ҫ�Զ������������Ҫ���������������������͵��� StartAction �����������ö���
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}
void UMyActionComponent::RemoveAction(UMyAction* ActionToRemove)
{
	// ��� ActionToRemove �Ƿ������δ����
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	// �� Actions �������Ƴ� ActionToRemove
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
//������������һ������
bool UMyActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// ���� Actions ���飬���Ҳ�����ָ������ActionName�Ķ���
	for (UMyAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			// ���ö����Ƿ��������
			if (!Action->CanStart(Instigator))
			{
				// ����޷����������������Ϣ��������������һ������
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			// �����ǰ������ڷ������ϣ������������ RPC ��������
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

//��������ֹͣһ������
bool UMyActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	// ���� Actions ���飬���Ҳ�ָֹͣ�����ƵĶ���
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
// ʵ�ַ����� RPC ���� ServerStartAction������ StartActionByName ��������
void UMyActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}
/*ReplicateSubobjects��һ���麯����������Unreal Engine�д����Ӷ�������縴�ơ�
����һ���̳���AActor��UActorComponent���࣬���Ҹ��������Ҫ�������н��и��Ƶ��Ӷ���ʱ��
��Ҫ����ReplicateSubobjects����������������ڶ�����θ����Ӷ����Ա�֤�������Ϳͻ���֮�����Ϸ״̬ͬ����*/
bool UMyActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	// ����Actions�����е�ÿһ��UMyAction����
	for (UMyAction* Action : Actions)
	{
		if (Action)
		{
			// ����Channel->ReplicateSubobject����Ϊ��ǰAction����������縴��
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	// �����Ƿ�ɹ��������κ��Ӷ���
	return WroteSomething;
}
//��������
void UMyActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyActionComponent, Actions);
}