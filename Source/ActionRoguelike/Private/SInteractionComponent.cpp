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

// 查找最佳可交互对象
void USInteractionComponent::FindBestInteractable()
{
	// 获取调试绘制交互状态的开关值
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	// 添加要查询的对象类型
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	// 获取视点位置和旋转
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	// 执行射线检测，获取所有可交互对象
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	// 根据检测结果设置调试绘制的颜色
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	// 清空焦点对象引用
	FocusedActor = nullptr;
	// 遍历所有可交互对象，获取第一个实现了 USGameplayInterface 接口的对象
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// 如果HitActor可以被交互，令FocusedActor为当前HitActor
			if (HitActor->Implements<USInteractionInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	// 如果存在焦点对象，则创建或更新用户界面
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
	// 否则移除用户界面
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
// 定义主动交互函数，调用ServerInteract函数进行交互
void USInteractionComponent::PrimaryInteract()
{
	// 发送RPC调用请求
	ServerInteract(FocusedActor);
}
// 定义在服务器上执行的RPC函数
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	// 检查是否选中了交互对象
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}
	// 获取拥有该组件的Actor对象所属的Pawn对象
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISInteractionInterface::Execute_Interact(InFocus, MyPawn);
}