// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

// ʵ��AMyPlayerController��SetPawn����
void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// �㲥OnPawnChanged�¼����������õĽ�ɫ��Ϊ����
	OnPawnChanged.Broadcast(InPawn);
}