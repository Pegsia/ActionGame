// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TioGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UTioSaveGame;

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

	/* Save Game */
	UPROPERTY()
	UTioSaveGame* CurrentSaveGame;

	FString SlotName;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	UFUNCTION(Exec)
	void KillAllBots();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	ATioGameModeBase();

	//~ Begin GameModeBase Interface
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//~ End GameModeBase Interface
};
