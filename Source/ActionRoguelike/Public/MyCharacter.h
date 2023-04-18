// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UMyAttributeComponent;
class UAnimMontage;
UCLASS()
class ACTIONROGUELIKE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	//相机组件
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	//抛体组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Projectileclass;

	//交互组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	//属性组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UMyAttributeComponent* AttributeComp;

	//攻击动画
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//攻击时TimerHandle，处理动画导致的攻击生成位置错误
	FTimerHandle TimerHandle_PrimaryAttack;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void JumpStart();
	void JumpEnd();
	void PickUp();
	void PrimaryAttack_TimeElapsed();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
