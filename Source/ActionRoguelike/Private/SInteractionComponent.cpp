// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SInteractionInterface.h"
#include "DrawDebugHelpers.h"
#include "MyWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

// ������ѿɽ�������
void USInteractionComponent::FindBestInteractable()
{
	// ��ȡ���Ի��ƽ���״̬�Ŀ���ֵ
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	// ���Ҫ��ѯ�Ķ�������
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	// ��ȡ�ӵ�λ�ú���ת
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	// ִ�����߼�⣬��ȡ���пɽ�������
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	// ���ݼ�������õ��Ի��Ƶ���ɫ
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	// ��ս����������
	FocusedActor = nullptr;
	// �������пɽ������󣬻�ȡ��һ��ʵ���� USGameplayInterface �ӿڵĶ���
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// ���HitActor���Ա���������FocusedActorΪ��ǰHitActor
			if (HitActor->Implements<USInteractionInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	// ������ڽ�������򴴽�������û�����
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UMyWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	// �����Ƴ��û�����
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}
// ����������������������ServerInteract�������н���
void USInteractionComponent::PrimaryInteract()
{
	// ����RPC��������
	ServerInteract(FocusedActor);
}
// �����ڷ�������ִ�е�RPC����
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	// ����Ƿ�ѡ���˽�������
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}
	// ��ȡӵ�и������Actor����������Pawn����
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISInteractionInterface::Execute_Interact(InFocus, MyPawn);
}