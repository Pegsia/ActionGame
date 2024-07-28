// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/TioActionEffect.h"
#include "TioActionEffect_Stunned.generated.h"

UCLASS()
class ACTIONGAME_API UTioActionEffect_Stunned : public UTioActionEffect
{
	GENERATED_BODY()
	
public:
	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;
};
