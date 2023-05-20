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
    // ����һ�������࣬����AActor������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info") 
	TSubclassOf<AActor> MonsterClass;
	// ����һ�����飬�洢�������ִ�еĶ�������Щ������UMyAction������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<UMyAction>> Actions;
    // ����һ��2D������ʾ�����ͼ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	// ����һ����������������FPrimaryAssetId����
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// ����һ��FPrimaryAssetId���͵Ķ��󣬸ö�������Ψһ��ʶһ����Ҫ�ʲ���
		// ����ʹ�õ��Ǳ�����������(GetFName())�����ַ���"Monsters"����������Ψһ��ʶ����
		return FPrimaryAssetId("Monsters", GetFName()); 
	}
};
