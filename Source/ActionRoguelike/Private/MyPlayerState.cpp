// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::AddCredits(int32 Delta)
{
    // ��ֹ�û���Ӹ�������
    if (!ensure(Delta >= 0.0f))
    {
        return;
    }
    // ���ӻ���Delta
    Credits += Delta;
    // ���� OnCreditsChanged ί���е��¼�
    OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AMyPlayerState::RemoveCredits(int32 Delta)
{
    // ��ֹ�û���ӻ��ȥ��������
    if (!ensure(Delta >= 0.0f))
    {
        return false;
    }
    // �����ҵ�ǰ�Ļ��� Credits С�� Delta���򷵻� false
    if (Credits < Delta)
    {
        return false;
    }
    // ���ٻ��� Delta
    Credits -= Delta;
    // ���� OnCreditsChanged ί���е��¼�
    OnCreditsChanged.Broadcast(this, Credits, -Delta);
    return true;
}
