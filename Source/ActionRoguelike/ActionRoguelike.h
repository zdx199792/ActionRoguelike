// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// ʹ��DECLARE_STATS_GROUP��������һ����Ϊ"STANFORD_Game"��ͳ���顣
// ���ͳ�������ڽ���ص�����ͳ����Ϣ���з��飬�������ܷ����н�����֯����ʾ
// �������£�
// TEXT("STANFORD_Game")��ָ��ͳ��������ƣ���TEXT�꽫�ַ���ת��ΪFText���͡�
// STATGROUP_STANFORD��ָ��ͳ��������Ʊ�ʶ���������ڴ��������ø�ͳ���顣
// STATCAT_Advanced��ָ��ͳ����ķ��࣬���������ܷ��������н��з�����ʾ��
DECLARE_STATS_GROUP(TEXT("STANFORD_Game"), STATGROUP_STANFORD, STATCAT_Advanced);

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