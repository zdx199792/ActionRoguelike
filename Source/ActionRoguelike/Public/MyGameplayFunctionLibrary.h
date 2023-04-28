// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//实现对目标角色应用伤害
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	//实现对目标角色应用方向性伤害
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
};