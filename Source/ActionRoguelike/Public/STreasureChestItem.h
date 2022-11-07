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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
