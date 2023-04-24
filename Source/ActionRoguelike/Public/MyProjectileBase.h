// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;

UCLASS(ABSTRACT)	//声明为抽象基类，防止在游戏过程中创建该类的实例
class ACTIONROGUELIKE_API AMyProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	//碰撞效果震动的类
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	//碰撞效果震动的内部半径
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;
	//碰撞效果震动的外部半径
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
	//碰撞效果音效
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound;
	//碰撞效果粒子系统
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	//球体组件用于判断碰撞
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	//添加抛体组件用于运动
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	//添加粒子系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	//添加音频组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;
	
	//声明了一个用于处理 Actor 碰撞事件的函数
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    //声明了一个蓝图可调用的、Native 实现的事件函数，用于处理爆炸效果。
	UFUNCTION(BluePrintCallable, BlueprintNativeEvent)
	void Explode();
	
    //类的组件初始化完成后被调用的函数
	virtual void PostInitializeComponents() override;
public:	
	// Sets default values for this actor's properties
	AMyProjectileBase();
};
