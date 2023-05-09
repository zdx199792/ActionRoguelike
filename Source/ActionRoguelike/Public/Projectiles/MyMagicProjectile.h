// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectileBase.h"
#include "GameplayTagContainer.h"
#include "MyMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UMyActionEffect;
UCLASS()
class ACTIONROGUELIKE_API AMyMagicProjectile : public AMyProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMagicProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
		
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UMyActionEffect> BurningActionClass;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};