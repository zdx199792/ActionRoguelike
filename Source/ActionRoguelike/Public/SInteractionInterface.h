// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SInteractionInterface.generated.h"

// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class USInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


//提供交互的API
class ACTIONROGUELIKE_API ISInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//交互功能函数，参数表示交互的对象
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
	/* Called after the Actor state was restored from a SaveGame file. */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	// 获取交互信息文本
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};
