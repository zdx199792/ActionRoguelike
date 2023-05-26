// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChestItem.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASTreasureChestItem::ASTreasureChestItem()
{
	//��������ײ�Mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	//�����������Mesh
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	//�򿪱���ʱ������б�Ƕ�
	OpenPitch = 110.0f;
	// ��������
	bReplicates = true;
}

void ASTreasureChestItem::Interact_Implementation(APawn* InstigatorPawn)
{
	// �л�����״̬
	bLidOpened = !bLidOpened;
	// ���ؿͻ���ҲҪͬ���������״̬
	OnRep_LidOpened();
}
void ASTreasureChestItem::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}
void ASTreasureChestItem::OnRep_LidOpened()
{
	// ���ݸ���״̬���ø��ӵ���ת�Ƕ�
	float CurrPitch = bLidOpened ? OpenPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

// ��GetLifetimeReplicatedProps������������Ҫͬ��������
void ASTreasureChestItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// �� bLidOpened ������ӵ���Ҫ���Ƶ������б���
	DOREPLIFETIME(ASTreasureChestItem, bLidOpened);
}