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
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};
UCLASS(Blueprintable)	// ����UMyActionΪһ������ͼ�����࣬����UE��ͼϵͳ
class ACTIONROGUELIKE_API UMyAction : public UObject
{
	GENERATED_BODY()
protected:
	// Buff״̬ͼ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	// ������ʼ��ʱ��
	UPROPERTY(Replicated)
	float TimeStarted;

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
	//RepData���Խ��������縴�ƣ��������Է����仯ʱ���ɷ��������յ����£�����������ΪOnRep_RepData�ĺ���
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
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
	//�ж϶����Ƿ�֧�����縴��
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
