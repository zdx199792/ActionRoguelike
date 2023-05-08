// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
    // ȷ��WorldContext�ǿգ������˳�����
    if (!ensure(WorldContext))
    {
        return;
    }
    // ��WorldContext��ȡUWorld����
    UWorld* World = WorldContext->GetWorld();
    // ȷ��UWorld����ǿգ������˳�����
    if (!ensure(World))
    {
        return;
    }
    // �жϵ�ǰ����ģʽ�Ƿ�Ϊ�ͻ���ģʽ������ģʽ������Ϣǰ׺
    FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
    // ���GEngine�������
    if (GEngine)
    {
        // ʹ��GEngine������ǰ׺����Ϣ��ӵ���Ļ����ʾ
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
    }
}