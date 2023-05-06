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

	//重写互动接口的互动方法
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	//表示盖子是否打开，并通过RepNotify标记实现了状态同步,RepNotify是一个属性标记，用于表示在属性更新时应该调用哪个函数。
	//ReplicatedUsing 参数指定一个名为 OnRep_LidOpened 的函数，当该属性的值在网络上改变时会调用该函数，以使客户端和服务器上的值保持同步。
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
	bool bLidOpened;
	// OnRep函数在客户端和服务器同步变量后调用
	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
};
