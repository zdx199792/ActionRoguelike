// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class UMyWorldUserWidget;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
    USInteractionComponent();
    // 主动交互函数
    void PrimaryInteract();
protected:
    virtual void BeginPlay() override;
    // 查找最佳可交互对象
    void FindBestInteractable();
    // 当前焦点对象
    UPROPERTY()
    AActor* FocusedActor;
    // 跟踪射线的距离
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceDistance;
    // 跟踪射线的半径
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceRadius;
    // 跟踪射线的碰撞通道
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    TEnumAsByte<ECollisionChannel> CollisionChannel;
    // 默认用户界面类
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMyWorldUserWidget> DefaultWidgetClass;
    // 默认用户界面实例
    UPROPERTY()
    UMyWorldUserWidget* DefaultWidgetInstance;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
