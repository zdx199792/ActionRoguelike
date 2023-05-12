// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::TogglePauseMenu()
{
	// 检查暂停菜单是否存在且在视口中
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		// 如果是，将其从父类中移除，并将其指针设为null
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		// 如果是，将其从父类中移除，并将其指针设为null
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	// 否则，创建暂停菜单的一个实例
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		// 否则，创建暂停菜单的一个实例
		PauseMenuInstance->AddToViewport(100);
		// 显示鼠标光标并将输入模式设为只接受UI输入
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 将"PauseMenu"动作绑定到TogglePauseMenu函数。当按键被按下时，TogglePauseMenu函数将被调用
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMyPlayerController::TogglePauseMenu);
}
// 实现AMyPlayerController的SetPawn函数
void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// 广播OnPawnChanged事件，将新设置的角色作为参数
	OnPawnChanged.Broadcast(InPawn);
}
void AMyPlayerController::BeginPlayingState()
{
	// 调用蓝图中实现的BlueprintBeginPlayingState方法
	BlueprintBeginPlayingState();
}


void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 广播OnPlayerStateReceived事件，将PlayerState作为参数传递
	OnPlayerStateReceived.Broadcast(PlayerState);
}