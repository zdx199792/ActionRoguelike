#include "Pickups/MyPickup.h"
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
	bIsActive = true;
	SetReplicates(true);
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

// 设置物品拾取的激活状态
void AMyPickup::SetPickupState(bool bNewIsActive)
{
    // 更新激活状态
    bIsActive = bNewIsActive;
    // 调用OnRep_IsActive方法，以应用新的激活状态
    OnRep_IsActive();
}
void AMyPickup::OnRep_IsActive()
{
    // 根据激活状态设置碰撞检测
    SetActorEnableCollision(bIsActive);
    // 设置根组件和所有子组件的可见性
    RootComponent->SetVisibility(bIsActive, true);
}

// 获取需要在网络中复制的属性
void AMyPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // 添加bIsActive属性以在网络中进行复制
    DOREPLIFETIME(AMyPickup, bIsActive);
}