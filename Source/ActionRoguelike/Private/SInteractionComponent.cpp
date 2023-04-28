// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SInteractionInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//�������н���
void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	//��ײ��ѯ����
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); //WorldDynamic����ʱ���

	AActor* Owner = GetOwner();						//��ȡ��ɫӵ����
	FVector EyeLocation;
	FRotator EyeRotation;
	//ʵ�δ��ݣ��޸�Location��Rotation����EyeLocation��ΪStart
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	//����End
	FVector End = EyeLocation + (EyeRotation.Vector()*1000);

	//FHitResult Hit;		//��ײ���
	//���߼�⣻����: ���н��(FHitReslut)����ʼλ�á���ֹλ�á���ײ��ѯ����(FCollisionObjectQueryParams)
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);


	bool bBlockHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LinearColors = bBlockHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits) 
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.0f, 32, LinearColors, false, 2.0f);
		}
		AActor* HitActor = Hit.GetActor();
		if (HitActor) 	
		{
			//���HitActorʵ���˽����ӿ���USInteractionInterface
			if (HitActor->Implements<USInteractionInterface>()) 
			{
				//OwnerǿתΪAPawn
				APawn* MyPawn = Cast<APawn>(Owner);
				//����ʵ�ֵĽӿ�
				ISInteractionInterface::Execute_Interact(HitActor, MyPawn);
			}
			//�����׸������������ѭ��
			break; 
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LinearColors, false, 2.0f, 0, 2.0f);
	}
}
