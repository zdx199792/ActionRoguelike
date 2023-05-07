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

UCLASS(ABSTRACT)	//����Ϊ������࣬��ֹ����Ϸ�����д��������ʵ��
class ACTIONROGUELIKE_API AMyProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	//��ײЧ���𶯵���
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	//��ײЧ���𶯵��ڲ��뾶
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;
	//��ײЧ���𶯵��ⲿ�뾶
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
	//��ײЧ����Ч
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound;
	//��ײЧ������ϵͳ
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	//������������ж���ײ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	//���������������˶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	//�������ϵͳ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	//�����Ƶ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;
	
	//������һ�����ڴ��� Actor ��ײ�¼��ĺ���
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    //������һ����ͼ�ɵ��õġ�Native ʵ�ֵ��¼����������ڴ���ըЧ����
	UFUNCTION(BluePrintCallable, BlueprintNativeEvent)
	void Explode();
	
    //��������ʼ����ɺ󱻵��õĺ���
	virtual void PostInitializeComponents() override;
public:	
	// Sets default values for this actor's properties
	AMyProjectileBase();
};
