// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"
class UMySaveGame;
//ʹ�û����¼�������UIҪ����ֱ�ӽ�Credit�󶨵�UI�ϣ����߻�ÿһ֡����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AMyPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
UCLASS()
class ACTIONROGUELIKE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "PlayerState|Credits")
	int32 Credits;
	// OnRep_Credits��������һ����ΪOldCredits��int32���Ͳ�������ʾ���Եľ�ֵ
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

	//�������״̬
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UMySaveGame* SaveObject);
	//�������״̬
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UMySaveGame* SaveObject);
};
