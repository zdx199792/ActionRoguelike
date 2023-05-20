// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PrimaryAssetLabel.h"
#include "MyMonsterData.generated.h"

class UMyAction;

UCLASS()
class ACTIONROGUELIKE_API UMyMonsterData : public UPrimaryAssetLabel
{
	GENERATED_BODY()
public:
    // 定义一个怪物类，它是AActor的子类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info") 
	TSubclassOf<AActor> MonsterClass;
	// 定义一个数组，存储怪物可以执行的动作，这些动作是UMyAction的子类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<UMyAction>> Actions;
    // 定义一个2D纹理，表示怪物的图标
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	// 定义一个常量函数，返回FPrimaryAssetId类型
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// 返回一个FPrimaryAssetId类型的对象，该对象用于唯一标识一个主要资产。
		// 这里使用的是本类对象的名称(GetFName())，和字符串"Monsters"组合生成这个唯一标识符。
		return FPrimaryAssetId("Monsters", GetFName()); 
	}
};
