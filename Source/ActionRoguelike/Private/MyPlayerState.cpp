// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::AddCredits(int32 Delta)
{
    // 防止用户添加负数或零
    if (!ensure(Delta >= 0.0f))
    {
        return;
    }
    // 增加积分Delta
    Credits += Delta;
    // 触发 OnCreditsChanged 委托中的事件
    OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AMyPlayerState::RemoveCredits(int32 Delta)
{
    // 防止用户添加或减去负数或零
    if (!ensure(Delta >= 0.0f))
    {
        return false;
    }
    // 如果玩家当前的积分 Credits 小于 Delta，则返回 false
    if (Credits < Delta)
    {
        return false;
    }
    // 减少积分 Delta
    Credits -= Delta;
    // 触发 OnCreditsChanged 委托中的事件
    OnCreditsChanged.Broadcast(this, Credits, -Delta);
    return true;
}
