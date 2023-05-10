// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"
class UMySaveGame;
//使用基于事件来更新UI要优于直接将Credit绑定到UI上，后者会每一帧调用
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AMyPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
UCLASS()
class ACTIONROGUELIKE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "PlayerState|Credits")
	int32 Credits;
	// OnRep_Credits函数接收一个名为OldCredits的int32类型参数，表示属性的旧值
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Credits") // < Category|SubCategory
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Credits")
	bool RemoveCredits(int32 Delta);
		
	UFUNCTION(BlueprintCallable, Category = "PlayerState|Credits")
	int32 GetCredits() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	//保存玩家状态
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UMySaveGame* SaveObject);
	//加载玩家状态
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UMySaveGame* SaveObject);
};
