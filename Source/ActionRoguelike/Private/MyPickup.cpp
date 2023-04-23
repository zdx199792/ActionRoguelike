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

//����������Ʒ�ļ���״̬���Ƿ�ɽ�������Ⱦ��
void AMyPickup::SetPickupState(bool bNewIsActive)
{
	//������Ʒ�Ƿ�ɽ���
	SetActorEnableCollision(bNewIsActive);
	//������Ʒ�Ƿ����Ⱦ
	RootComponent->SetVisibility(bNewIsActive, true);
}
