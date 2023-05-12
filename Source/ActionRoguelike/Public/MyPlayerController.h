// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
// 声明一个具有一个参数的动态多播委托类型FOnPawnChanged，当玩家Pawn变化时触发
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
// 声明一个具有一个参数的动态多播委托，用于通知PlayerState改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);
UCLASS()
class ACTIONROGUELIKE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	// 用于指定暂停菜单的蓝图类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	// 一个UUserWidget对象的指针，它将在运行时保存暂停菜单的实例
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	// 一个函数，用于切换暂停菜单的显示/隐藏状态
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	// 重写基类APlayerController的函数SetupInputComponent，用于设置玩家的输入绑定
	void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;

	//监听传入的玩家状态（对于客户端而言，在最初加入游戏时玩家状态可能为nullptr，
	//之后玩家状态将不再改变，因为PlayerController在整个关卡中保持相同的玩家状态）。
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;

	//当玩家控制器准备开始游戏时调用，这是初始化诸如UI之类的东西的合适时机，
	//如果在BeginPlay中初始化为时过早（尤其是在多人游戏客户端中，可能尚未接收到所有数据，例如PlayerState）
	virtual void BeginPlayingState() override;
	
	// 创建一个可以在蓝图中实现的BlueprintBeginPlayingState事件
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	// 重写OnRep_PlayerState方法，用于处理PlayerState的复制
	void OnRep_PlayerState() override;
};
