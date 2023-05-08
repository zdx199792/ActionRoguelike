// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "MyAction.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)	// 声明UMyAction为一个可蓝图化的类，用于UE蓝图系统
class ACTIONROGUELIKE_API UMyAction : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
	UMyActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UMyActionComponent* GetOwningComponent() const;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	//bIsRunning属性将进行网络复制，当该属性发生变化时（由服务器接收到更新），将调用名为OnRep_IsRunning的函数
	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();
public:
	void Initialize(UMyActionComponent* NewActionComp);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;
	//判断对象是否支持网络复制
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
