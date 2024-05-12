// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerUpActor_Credits.h"
#include <TioPlayerState.h>

ATioPowerUpActor_Credits::ATioPowerUpActor_Credits()
{
	CreditsAmount = 100;
}

void ATioPowerUpActor_Credits::Interact_Implementation(APawn* Insgitator)
{
	if (!ensure(Insgitator))
	{
		return;
	}

	ATioPlayerState* PS = Insgitator->GetPlayerState<ATioPlayerState>();
	if (PS)
	{
		PS->AddCredits(CreditsAmount);
		HideAndCoolDownPowerUp();
	}
}

