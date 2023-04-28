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

//主动进行交互
void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	//碰撞查询参数
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); //WorldDynamic类型时检测

	AActor* Owner = GetOwner();						//获取角色拥有者
	FVector EyeLocation;
	FRotator EyeRotation;
	//实参传递，修改Location和Rotation，将EyeLocation作为Start
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	//设置End
	FVector End = EyeLocation + (EyeRotation.Vector()*1000);

	//FHitResult Hit;		//碰撞结果
	//射线检测；参数: 命中结果(FHitReslut)、开始位置、终止位置、碰撞查询参数(FCollisionObjectQueryParams)
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
			//如果HitActor实现了交互接口类USInteractionInterface
			if (HitActor->Implements<USInteractionInterface>()) 
			{
				//Owner强转为APawn
				APawn* MyPawn = Cast<APawn>(Owner);
				//调用实现的接口
				ISInteractionInterface::Execute_Interact(HitActor, MyPawn);
			}
			//遇到首个交互体后跳出循环
			break; 
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LinearColors, false, 2.0f, 0, 2.0f);
	}
}
