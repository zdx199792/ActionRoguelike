// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
class UMyActionComponent;
UCLASS()
class ACTIONROGUELIKE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	// 表示角色是否处于被击晕状态
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;
	// 动作组件
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UMyActionComponent* ActionComp;
	// 重写UAnimInstance的NativeInitializeAnimation方法
	void NativeInitializeAnimation() override;
	// 重写UAnimInstance的NativeUpdateAnimation方法
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
