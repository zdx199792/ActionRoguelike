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
class UMyActionComponent;
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

	//交互组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	//属性组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UMyAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	void MoveForward(float value);
	void MoveRight(float value);
	void SprintStart();
	void SprintStop();
	void PrimaryAttack();	
	void BlackHoleAttack();
	void PickUp();
	void Dash();


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Exec宏，意味着该函数可以在游戏运行时通过控制台命令来调用
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
