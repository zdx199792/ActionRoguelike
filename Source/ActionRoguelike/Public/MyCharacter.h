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

	//魔法攻击抛体组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Projectileclass;

	//黑洞攻击抛体组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BlackHoleProjectileclass;

	//闪现抛体组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DashProjectileclass;

	//交互组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	//属性组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UMyAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyActionComponent* ActionComp;
	
	//攻击动画
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//攻击时TimerHandle，处理动画导致的攻击生成位置错误
	FTimerHandle TimerHandle_PrimaryAttack;

	//手部插槽名
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	//攻击动画时生成的粒子效果
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	void MoveForward(float value);
	void MoveRight(float value);
	void SprintStart();
	void SprintStop();
	void PrimaryAttack();
	void JumpStart();
	void JumpEnd();
	void PickUp();
	void PrimaryAttack_TimeElapsed();
	void Dash();
	void Dash_TimeElapsed();
	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	void StartAttackEffects();

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
