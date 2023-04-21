// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT)	//����Ϊ������࣬��ֹ����Ϸ�����д��������ʵ��
class ACTIONROGUELIKE_API AMyProjectileBase : public AActor
{
	GENERATED_BODY()

protected:

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
