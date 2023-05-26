// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChestItem.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASTreasureChestItem::ASTreasureChestItem()
{
	//创建宝箱底部Mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	//创建宝箱盖子Mesh
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	//打开宝箱时盖子倾斜角度
	OpenPitch = 110.0f;
	// 开启复制
	bReplicates = true;
}

void ASTreasureChestItem::Interact_Implementation(APawn* InstigatorPawn)
{
	// 切换盖子状态
	bLidOpened = !bLidOpened;
	// 本地客户端也要同步宝箱盖子状态
	OnRep_LidOpened();
}
void ASTreasureChestItem::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}
void ASTreasureChestItem::OnRep_LidOpened()
{
	// 根据盖子状态设置盖子的旋转角度
	float CurrPitch = bLidOpened ? OpenPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

// 在GetLifetimeReplicatedProps函数中声明需要同步的属性
void ASTreasureChestItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 将 bLidOpened 属性添加到需要复制的属性列表中
	DOREPLIFETIME(ASTreasureChestItem, bLidOpened);
}