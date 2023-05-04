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
	// ���� DefaultActions ���飬�����е�ÿ��������ӵ� Actions ������
	for (TSubclassOf<UMyAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UMyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// ��Ӷ���
void UMyActionComponent::AddAction(TSubclassOf<UMyAction> ActionClass)
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
	}
}

//������������һ������
bool UMyActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// ���� Actions ���飬���Ҳ�����ָ������ActionName�Ķ���
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

//��������ֹͣһ������
bool UMyActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	// ���� Actions ���飬���Ҳ�ָֹͣ�����ƵĶ���
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
