// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TioAICharacter.h"
#include "EngineUtils.h"
#include "Curves/CurveFloat.h"

ATioGameModeBase::ATioGameModeBase()
{
	SpawnBotInterval = 2.f;

}

void ATioGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpwanBot, this, &ATioGameModeBase::SpawnBotElapsed, SpawnBotInterval, true);
}

void ATioGameModeBase::SpawnBotElapsed()
{
	int32 NrOfLiveBots = 0;
	for (TActorIterator<ATioAICharacter> It(GetWorld()); It; ++It)
	{
		ATioAICharacter* AICharacter = *It;
		if (AICharacter->IsAlive())
		{
			++NrOfLiveBots;
		}
	}

	float MaxBotCount = 10.f;
	if (DifficultCurve)
	{
		MaxBotCount = DifficultCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NrOfLiveBots >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, QueryBotSpawn, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATioGameModeBase::OnQueryCompleted);
	}
}

void ATioGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Spawn Bot EQS Failed "));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

