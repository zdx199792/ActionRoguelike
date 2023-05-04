// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction.h"

void UMyAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* UMyAction::GetWorld() const
{	
    // ��ȡ�ö������ڵ���������
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
        // �����������Ϊ UActorComponent ���ͣ��ͷ��������ڵ������ָ��
		return Comp->GetWorld();
	}
    // ����޷���ȡ������������������ָ�룬�򷵻� nullptr
	return nullptr;
}
