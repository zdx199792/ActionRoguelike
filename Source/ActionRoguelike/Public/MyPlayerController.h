// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
// 声明一个具有一个参数的动态多播委托类型FOnPawnChanged，当玩家Pawn变化时触发
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
