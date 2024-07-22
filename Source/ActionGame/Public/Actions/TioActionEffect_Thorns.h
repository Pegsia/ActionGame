// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/TioActionEffect.h"
#include "TioActionEffect_Thorns.generated.h"

class UTioAttributeComponent;

UCLASS()
class ACTIONGAME_API UTioActionEffect_Thorns : public UTioActionEffect
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float ThornsFraction;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta);

public:
	UTioActionEffect_Thorns();

	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;
};
