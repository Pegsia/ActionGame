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
	/* Credits */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	/* Bot Spawn */
	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	UCurveFloat* DifficultCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	UEnvQuery* QueryBotSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	float SpawnBotInterval;

	FTimerHandle TimerHandle_SpwanBot;

	void SpawnBotElapsed();

	UFUNCTION()
	void OnBotSpwanQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	/* Power Up Spawn */
	UPROPERTY(EditDefaultsOnly, Category = "PowerUpSpawn")
	float RequiredPowerUpDistance;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUpSpawn")
	int32 DesiredPowerUpCount;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUpSpawn")
	TArray<TSubclassOf<AActor>> PowerUpClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUpSpawn")
	UEnvQuery* QueryPowerUpSpawn;

	UFUNCTION()
	void OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	/* Restart Player */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerRespawn")
	float RespawnTime;

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAllBots();

	ATioGameModeBase();

	virtual void StartPlay() override;
};
