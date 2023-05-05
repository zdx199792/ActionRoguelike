// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInteractionInterface.h"
#include "MyPickup.generated.h"

class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API AMyPickup : public AActor, public ISInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPickup();
	void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	
	//���ڴ�����ߵ���ײ�ͽ���
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	//������Ⱦ���ߵ����
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	//����������ʱ����
	UPROPERTY(EditAnywhere, Category = "Pickups")
	float RespawnTime;

	//���ڹ�����Ʒ�����Ķ�ʱ��
	FTimerHandle TimerHandle_RespawnTimer;

	//������ʾ����
	UFUNCTION()
	void ShowPickup();
	
	//�������ص���
	void HideAndCooldownPickup();

	//���õ��ߵļ���״̬���Ƿ�ɽ�������Ⱦ��
	void SetPickupState(bool bNewIsActive);
};
