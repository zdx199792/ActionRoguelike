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
	//���ɱ����
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	//������
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	//ħ�������������
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Projectileclass;

	//�ڶ������������
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BlackHoleProjectileclass;

	//�����������
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DashProjectileclass;

	//�������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	//�������
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UMyAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyActionComponent* ActionComp;
	
	//��������
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//����ʱTimerHandle�����������µĹ�������λ�ô���
	FTimerHandle TimerHandle_PrimaryAttack;

	//�ֲ������
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	//��������ʱ���ɵ�����Ч��
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

	//Exec�꣬��ζ�Ÿú�����������Ϸ����ʱͨ������̨����������
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
