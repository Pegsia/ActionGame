// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/TioProjectileBase.h"
#include "TioDashProjectile.generated.h"

UCLASS()
class ACTIONGAME_API ATioDashProjectile : public ATioProjectileBase
{
	GENERATED_BODY()
	
public:
	ATioDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float TeleportDelay;

	FTimerHandle TimerHandle_Detonate;

	void Teleport();

	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;
};
