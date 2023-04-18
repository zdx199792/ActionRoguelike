// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTargetDummy.generated.h"

class UMyAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AMyTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	UMyAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta);

};
