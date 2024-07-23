// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPlayerState.h"
#include "TioSaveGame.h"

void ATioPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;
	OnCreditsChange.Broadcast(this, Credits, Delta);
}

bool ATioPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;
	OnCreditsChange.Broadcast(this, Credits, -Delta);
	return true;
}

void ATioPlayerState::LoadPlayerState_Implementation(UTioSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}

void ATioPlayerState::SavePlayerState_Implementation(UTioSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

int32 ATioPlayerState::GetCredits() const
{
	return Credits;
}