// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TioGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ACTIONGAME_API ATioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* QueryBotSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotInterval;

	FTimerHandle TimerHandle_SpwanBot;

	void SpawnBotElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	ATioGameModeBase();

	virtual void StartPlay() override;
};
