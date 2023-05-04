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
	//发射的 Projectile 类型
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	//手部的 socket 名称
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	//攻击动画的延迟
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	//攻击动画
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//攻击时播放的粒子特效
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	//攻击动画延迟结束后调用
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	//用于实现该功能类具体功能的函数
	virtual void StartAction_Implementation(AActor* Instigator) override;

	UMyAction_ProjectileAttack();
};
