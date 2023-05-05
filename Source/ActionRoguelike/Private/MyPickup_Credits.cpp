// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickup_Credits.h"
#include "MyPlayerState.h"



AMyPickup_Credits::AMyPickup_Credits()
{
	CreditsAmount = 80;
}

// 实现 Pickup 父类的虚函数，给玩家增加 CreditsAmount 数量的 Credits
void AMyPickup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AMyPlayerState* PS = InstigatorPawn->GetPlayerState<AMyPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPickup();
	}
}