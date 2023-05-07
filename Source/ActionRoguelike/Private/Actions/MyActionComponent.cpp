// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyActionComponent.h"
#include "Actions/MyAction.h"

// Sets default values for this component's properties
UMyActionComponent::UMyActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UMyActionComponent::BeginPlay()
{
	Super::BeginPlay();
	// ���� DefaultActions ���飬�����е�ÿ��������ӵ� Actions ������
	for (TSubclassOf<UMyAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UMyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

// ��Ӷ���
void UMyActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMyAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	// ����һ���µĶ���������ӵ� Actions ������
	UMyAction* NewAction = NewObject<UMyAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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