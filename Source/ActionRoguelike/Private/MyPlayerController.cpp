// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::TogglePauseMenu()
{
	// �����ͣ�˵��Ƿ���������ӿ���
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		// ����ǣ�����Ӹ������Ƴ���������ָ����Ϊnull
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		// ����ǣ�����Ӹ������Ƴ���������ָ����Ϊnull
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	// ���򣬴�����ͣ�˵���һ��ʵ��
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		// ���򣬴�����ͣ�˵���һ��ʵ��
		PauseMenuInstance->AddToViewport(100);
		// ��ʾ����겢������ģʽ��Ϊֻ����UI����
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// ��"PauseMenu"�����󶨵�TogglePauseMenu������������������ʱ��TogglePauseMenu������������
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMyPlayerController::TogglePauseMenu);
}
// ʵ��AMyPlayerController��SetPawn����
void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// �㲥OnPawnChanged�¼����������õĽ�ɫ��Ϊ����
	OnPawnChanged.Broadcast(InPawn);
}
void AMyPlayerController::BeginPlayingState()
{
	// ������ͼ��ʵ�ֵ�BlueprintBeginPlayingState����
	BlueprintBeginPlayingState();
}


void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// �㲥OnPlayerStateReceived�¼�����PlayerState��Ϊ��������
	OnPlayerStateReceived.Broadcast(PlayerState);
}