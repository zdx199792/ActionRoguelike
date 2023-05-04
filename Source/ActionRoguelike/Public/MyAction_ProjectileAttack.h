// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "MyAction_ProjectileAttack.generated.h"
class UAnimMontage;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyAction_ProjectileAttack : public UMyAction
{
	GENERATED_BODY()
protected:
	//����� Projectile ����
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	//�ֲ��� socket ����
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	//�����������ӳ�
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	//��������
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//����ʱ���ŵ�������Ч
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	//���������ӳٽ��������
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	//����ʵ�ָù�������幦�ܵĺ���
	virtual void StartAction_Implementation(AActor* Instigator) override;

	UMyAction_ProjectileAttack();
};
