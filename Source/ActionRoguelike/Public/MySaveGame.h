// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	// 用于表示该结构体所属的Actor的名称
	UPROPERTY()
	FString ActorName;
	// 用于存储可移动Actor的位置、旋转和缩放信息
	UPROPERTY()
	FTransform Transform;
	// 用于存储其他与Actor相关的字节数据
	UPROPERTY()
	TArray<uint8> ByteData;
};
UCLASS()
class ACTIONROGUELIKE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	// 用于存储游戏中的Credits数据
	UPROPERTY()
	int32 Credits;
	// 用于存储游戏中多个Actor的保存数据
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
