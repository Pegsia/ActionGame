// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/TioPowerUpActor.h"
#include "TioPowerUpActor_Credits.generated.h"

UCLASS()
class ACTIONGAME_API ATioPowerUpActor_Credits : public ATioPowerUpActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsAmount;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

	ATioPowerUpActor_Credits();
};
