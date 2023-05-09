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
	// ���ڱ�ʾ�ýṹ��������Actor������
	UPROPERTY()
	FString ActorName;
	// ���ڴ洢���ƶ�Actor��λ�á���ת��������Ϣ
	UPROPERTY()
	FTransform Transform;
	// ���ڴ洢������Actor��ص��ֽ�����
	UPROPERTY()
	TArray<uint8> ByteData;
};
UCLASS()
class ACTIONROGUELIKE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	// ���ڴ洢��Ϸ�е�Credits����
	UPROPERTY()
	int32 Credits;
	// ���ڴ洢��Ϸ�ж��Actor�ı�������
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
