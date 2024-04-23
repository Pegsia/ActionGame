// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/TioPowerUpActor.h"
#include "TioPowerUpActor_HealthPotion.generated.h"

UCLASS()
class ACTIONGAME_API ATioPowerUpActor_HealthPotion : public ATioPowerUpActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Heal")
	float HealAmount;

public:
	ATioPowerUpActor_HealthPotion();
	
	void Interact_Implementation(APawn* InsgitatorPawn) override;
};
