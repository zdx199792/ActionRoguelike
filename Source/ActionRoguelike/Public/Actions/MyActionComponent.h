// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "MyActionComponent.generated.h"

class UMyAction;
// 定义一个带有两个参数的动态多播委托。当Action状态改变时触发
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UMyActionComponent*, OwningComp, UMyAction*, Action);
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
	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UMyAction>> DefaultActions;

	// 声明一个保存动作指针的数组
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UMyAction*> Actions;

	virtual void BeginPlay() override;

public:	
	// 声明一个可以在蓝图中分配的多播委托，当动作开始时触发。
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	// 声明一个可以在蓝图中分配的多播委托，当动作停止时触发。
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
