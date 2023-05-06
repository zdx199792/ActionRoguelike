// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyWorldUserWidget.generated.h"
class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	// NativeTick 函数是 UMyWorldUserWidget 类的虚函数，用于在用户界面蓝图每帧更新时执行自定义逻辑
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;
};
