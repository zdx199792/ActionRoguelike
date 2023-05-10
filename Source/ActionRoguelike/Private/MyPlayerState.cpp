// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MySaveGame.h"
#include "Net/UnrealNetwork.h"
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

int32 AMyPlayerState::GetCredits() const
{
    return Credits;
}
void AMyPlayerState::SavePlayerState_Implementation(UMySaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}


void AMyPlayerState::LoadPlayerState_Implementation(UMySaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		// 因为创建UI的时间推后了，通过AddCredits的方式给PlayerState的Credits赋值，确保积分修改事件触发
		AddCredits(SaveObject->Credits);
	}
}

void AMyPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, Credits);
}