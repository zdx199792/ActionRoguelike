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
	//��ͬ�������������Ч��ʵ�ֲ�ͬ����
}

//������ʾ��Ʒ�ĺ�����ͨ������ SetPickupState() ��������Ʒ����Ϊ�ɽ����Ϳ���Ⱦ��״̬
void AMyPickup::ShowPickup()
{
	SetPickupState(true);
}

//����������Ʒ��������Ʒ��������ʱ��
void AMyPickup::HideAndCooldownPickup()
{
	SetPickupState(false);

	//������һ����ʱ������ʱ���� ShowPickup() ��������������Ʒ
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AMyPickup::ShowPickup, RespawnTime);
}

// ������Ʒʰȡ�ļ���״̬
void AMyPickup::SetPickupState(bool bNewIsActive)
{
    // ���¼���״̬
    bIsActive = bNewIsActive;
    // ����OnRep_IsActive��������Ӧ���µļ���״̬
    OnRep_IsActive();
}
void AMyPickup::OnRep_IsActive()
{
    // ���ݼ���״̬������ײ���
    SetActorEnableCollision(bIsActive);
    // ���ø����������������Ŀɼ���
    RootComponent->SetVisibility(bIsActive, true);
}

// ��ȡ��Ҫ�������и��Ƶ�����
void AMyPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // ���bIsActive�������������н��и���
    DOREPLIFETIME(AMyPickup, bIsActive);
}