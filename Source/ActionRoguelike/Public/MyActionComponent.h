// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "MyActionComponent.generated.h"

class UMyAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UMyActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UMyAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UMyAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UMyAction* GetAction(TSubclassOf<UMyAction> ActionClass) const;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UMyActionComponent();

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UMyAction>> DefaultActions;

	// 声明一个保存动作指针的数组
	UPROPERTY()
	TArray<UMyAction*> Actions;

	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
