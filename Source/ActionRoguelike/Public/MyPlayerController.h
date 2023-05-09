// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
// ����һ������һ�������Ķ�̬�ಥί������FOnPawnChanged�������Pawn�仯ʱ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
UCLASS()
class ACTIONROGUELIKE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;
};
