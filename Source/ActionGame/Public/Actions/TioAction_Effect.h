// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/TioAction.h"
#include "TioAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UTioAction_Effect : public UTioAction
{
	GENERATED_BODY()
	
public:
	UTioAction_Effect();

	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* InstigatorActor);
};
