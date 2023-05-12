// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
// ����һ������һ�������Ķ�̬�ಥί������FOnPawnChanged�������Pawn�仯ʱ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
// ����һ������һ�������Ķ�̬�ಥί�У�����֪ͨPlayerState�ı��¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);
UCLASS()
class ACTIONROGUELIKE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	// ����ָ����ͣ�˵�����ͼ��
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	// һ��UUserWidget�����ָ�룬����������ʱ������ͣ�˵���ʵ��
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	// һ�������������л���ͣ�˵�����ʾ/����״̬
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	// ��д����APlayerController�ĺ���SetupInputComponent������������ҵ������
	void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;

	//������������״̬�����ڿͻ��˶��ԣ������������Ϸʱ���״̬����Ϊnullptr��
	//֮�����״̬�����ٸı䣬��ΪPlayerController�������ؿ��б�����ͬ�����״̬����
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;

	//����ҿ�����׼����ʼ��Ϸʱ���ã����ǳ�ʼ������UI֮��Ķ����ĺ���ʱ����
	//�����BeginPlay�г�ʼ��Ϊʱ���磨�������ڶ�����Ϸ�ͻ����У�������δ���յ��������ݣ�����PlayerState��
	virtual void BeginPlayingState() override;
	
	// ����һ����������ͼ��ʵ�ֵ�BlueprintBeginPlayingState�¼�
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	// ��дOnRep_PlayerState���������ڴ���PlayerState�ĸ���
	void OnRep_PlayerState() override;
};
