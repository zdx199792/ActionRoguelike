// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInteractionInterface.h"
#include "STreasureChestItem.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASTreasureChestItem : public AActor, public ISInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTreasureChestItem();

	UPROPERTY(EditAnywhere)
	float OpenPitch;

	//��д�����ӿڵĻ�������
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	//��ʾ�����Ƿ�򿪣���ͨ��RepNotify���ʵ����״̬ͬ��,RepNotify��һ�����Ա�ǣ����ڱ�ʾ�����Ը���ʱӦ�õ����ĸ�������
	//ReplicatedUsing ����ָ��һ����Ϊ OnRep_LidOpened �ĺ������������Ե�ֵ�������ϸı�ʱ����øú�������ʹ�ͻ��˺ͷ������ϵ�ֵ����ͬ����
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
	bool bLidOpened;
	// OnRep�����ڿͻ��˺ͷ�����ͬ�����������
	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
};
