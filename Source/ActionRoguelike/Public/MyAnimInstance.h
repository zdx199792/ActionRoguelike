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

	// ��ʾ��ɫ�Ƿ��ڱ�����״̬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;
	// �������
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UMyActionComponent* ActionComp;
	// ��дUAnimInstance��NativeInitializeAnimation����
	void NativeInitializeAnimation() override;
	// ��дUAnimInstance��NativeUpdateAnimation����
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
