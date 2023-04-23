#include "MyPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


AMyPickup::AMyPickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");
	RootComponent = SphereComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
}


void AMyPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	//不同道具子类根据其效果实现不同功能
}

//用于显示物品的函数，通过调用 SetPickupState() 函数将物品设置为可交互和可渲染的状态
void AMyPickup::ShowPickup()
{
	SetPickupState(true);
}

//用于隐藏物品并设置物品的重生定时器
void AMyPickup::HideAndCooldownPickup()
{
	SetPickupState(false);

	//设置了一个定时器，定时触发 ShowPickup() 函数用于重生物品
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AMyPickup::ShowPickup, RespawnTime);
}

//用于设置物品的激活状态（是否可交互和渲染）
void AMyPickup::SetPickupState(bool bNewIsActive)
{
	//设置物品是否可交互
	SetActorEnableCollision(bNewIsActive);
	//设置物品是否可渲染
	RootComponent->SetVisibility(bNewIsActive, true);
}
