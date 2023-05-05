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
    // ������������
    void PrimaryInteract();
protected:
    virtual void BeginPlay() override;
    // ������ѿɽ�������
    void FindBestInteractable();
    // ��ǰ�������
    UPROPERTY()
    AActor* FocusedActor;
    // �������ߵľ���
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceDistance;
    // �������ߵİ뾶
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceRadius;
    // �������ߵ���ײͨ��
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    TEnumAsByte<ECollisionChannel> CollisionChannel;
    // Ĭ���û�������
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMyWorldUserWidget> DefaultWidgetClass;
    // Ĭ���û�����ʵ��
    UPROPERTY()
    UMyWorldUserWidget* DefaultWidgetInstance;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
