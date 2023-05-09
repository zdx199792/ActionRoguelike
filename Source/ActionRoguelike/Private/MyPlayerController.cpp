// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

// 实现AMyPlayerController的SetPawn函数
void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// 广播OnPawnChanged事件，将新设置的角色作为参数
	OnPawnChanged.Broadcast(InPawn);
}