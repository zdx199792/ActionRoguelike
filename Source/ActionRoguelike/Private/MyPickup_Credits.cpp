// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickup_Credits.h"
#include "MyPlayerState.h"



AMyPickup_Credits::AMyPickup_Credits()
{
	CreditsAmount = 80;
}

// ʵ�� Pickup ������麯������������� CreditsAmount ������ Credits
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