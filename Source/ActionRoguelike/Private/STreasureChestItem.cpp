// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChestItem.h"
#include "Components/SceneComponent.h"

// Sets default values
ASTreasureChestItem::ASTreasureChestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��������ײ�Mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	//�����������Mesh
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	//�򿪱���ʱ������б�Ƕ�
	OpenPitch = 110.0f;

}

// Called when the game starts or when spawned
void ASTreasureChestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTreasureChestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTreasureChestItem::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(OpenPitch, 0, 0));
}

