// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPickup.h"
#include "MyPickup_Health.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyPickup_Health : public AMyPickup
{
	GENERATED_BODY()
	
protected:

public:
	//��д�ӿں�����ʵ�������ָ�ҩˮ�Ĺ���
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	AMyPickup_Health();
};