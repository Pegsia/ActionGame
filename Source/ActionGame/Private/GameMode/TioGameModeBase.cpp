// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TioAICharacter.h"
#include "EngineUtils.h"
#include "Curves/CurveFloat.h"
#include "TioCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All);
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("Tio.SpawnBots"), false, TEXT("Enable Spawn Bots via time"), ECVF_Cheat);

ATioGameModeBase::ATioGameModeBase()
{
	SpawnBotInterval = 2.f;
	RespawnTime = 2.f;
}

void ATioGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpwanBot, this, &ATioGameModeBase::SpawnBotElapsed, SpawnBotInterval, true);
}

void ATioGameModeBase::KillAllBots()
{
	for (TActorIterator<ATioAICharacter> It(GetWorld()); It; ++It)
	{
		ATioAICharacter* AICharacter = *It;

		if (AICharacter->IsAlive())
		{
			AICharacter->ApplyHealth(this);
		}
	}
}


void ATioGameModeBase::SpawnBotElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogGameModeBase, Log, TEXT("Bot spawn disable via 'CVar:Tio.SpawnBots'"));
		return;
	}

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
		UE_LOG(LogGameModeBase, Error, TEXT("GameMode Spawn Bot EQS Failed "));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

void ATioGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ATioCharacter* Player = Cast<ATioCharacter>(VictimActor);
	if (Player)
	{
		UE_LOG(LogGameModeBase, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
		FTimerHandle TimerHandle_PlayerRespawn;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_PlayerRespawn, Delegate, RespawnTime, false);
	}
}

void ATioGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

