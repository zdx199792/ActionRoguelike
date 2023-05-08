// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
    // 确保WorldContext非空，否则退出函数
    if (!ensure(WorldContext))
    {
        return;
    }
    // 从WorldContext获取UWorld对象
    UWorld* World = WorldContext->GetWorld();
    // 确保UWorld对象非空，否则退出函数
    if (!ensure(World))
    {
        return;
    }
    // 判断当前运行模式是否为客户端模式，根据模式设置消息前缀
    FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
    // 如果GEngine对象存在
    if (GEngine)
    {
        // 使用GEngine将带有前缀的消息添加到屏幕上显示
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
    }
}