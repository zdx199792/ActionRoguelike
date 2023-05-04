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
    // 获取该对象所在的容器对象
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
        // 如果容器对象为 UActorComponent 类型，就返回其所在的世界的指针
		return Comp->GetWorld();
	}
    // 如果无法获取到容器对象或者世界的指针，则返回 nullptr
	return nullptr;
}
