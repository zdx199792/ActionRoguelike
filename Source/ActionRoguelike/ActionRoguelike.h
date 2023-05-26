// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// 使用DECLARE_STATS_GROUP宏声明了一个名为"STANFORD_Game"的统计组。
// 这个统计组用于将相关的性能统计信息进行分组，并在性能分析中进行组织和显示
// 参数如下：
// TEXT("STANFORD_Game")：指定统计组的名称，用TEXT宏将字符串转换为FText类型。
// STATGROUP_STANFORD：指定统计组的名称标识符，用于在代码中引用该统计组。
// STATCAT_Advanced：指定统计组的分类，用于在性能分析工具中进行分类显示。
DECLARE_STATS_GROUP(TEXT("STANFORD_Game"), STATGROUP_STANFORD, STATCAT_Advanced);

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