// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TioAICharacter.h"
#include "EngineUtils.h"
#include "Curves/CurveFloat.h"
#include "TioCharacter.h"
#include "TioPlayerState.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TioSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "TioGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All);
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("Tio.SpawnBots"), false, TEXT("Enable Spawn Bots via time"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDrawInvaildPowerLocation(TEXT("Tio.InvaildPowerLocation"), true, TEXT("DrawInvaildPowerLocation"), ECVF_Cheat);

ATioGameModeBase::ATioGameModeBase()
{
	// Bot Spawn
	SpawnBotInterval = 2.f;
	RespawnTime = 5.f;

	// Power Up Spawn
	DesiredPowerUpCount = 8;
	RequiredPowerUpDistance = 100.f;

	// Credits
	CreditsPerKill = 20;

	// SaveGame
	SlotName = "SaveGame01";

	PlayerStateClass = ATioPlayerState::StaticClass();
}

void ATioGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}


void ATioGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Bot Spawn
	GetWorldTimerManager().SetTimer(TimerHandle_SpwanBot, this, &ATioGameModeBase::SpawnBotElapsed, SpawnBotInterval, true);

	// Power Up Spawn
	if (ensure(PowerUpClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, QueryPowerUpSpawn, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATioGameModeBase::OnPowerUpQueryCompleted);
		}
	}
}

void ATioGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	ATioPlayerState* PS = NewPlayer->GetPlayerState<ATioPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATioGameModeBase::OnBotSpwanQueryCompleted);
	}
}

void ATioGameModeBase::OnBotSpwanQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogGameModeBase, Error, TEXT("Spawn Bot EQS Failed"));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

void ATioGameModeBase::OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogGameModeBase, Error, TEXT("Spawn Power Up EQS Failed"));
		return;
	}

	bool bDrawInvaildPowerLocation = CVarDrawInvaildPowerLocation.GetValueOnGameThread();
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations; // log already used spawn locations
	
	// spawn while we still can
	int32 SpawnCount = 0;
	while (SpawnCount < DesiredPowerUpCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];

		bool bIsVaildLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerUpDistance)
			{
				if (bDrawInvaildPowerLocation)
				{
					DrawDebugSphere(GetWorld(), PickedLocation, 32, 16, FColor::Blue, false, 10.f);
				}
				bIsVaildLocation = false;
				break;
			}
		}

		if (!bIsVaildLocation)
		{
			continue;
		}

		int32 RandomPowerUpClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
		TSubclassOf<AActor> PickedClass = PowerUpClasses[RandomPowerUpClassIndex];

		AActor* NewPowerUpObj = GetWorld()->SpawnActor<AActor>(PickedClass, PickedLocation, FRotator::ZeroRotator);
		if (!NewPowerUpObj)
		{
			UE_LOG(LogGameModeBase, Error, TEXT("Spawn Power Up Actor Failed"));
			continue;
		}
		Locations.RemoveAt(RandomLocationIndex);
		UsedLocations.Add(PickedLocation);
		++SpawnCount;
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

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ATioPlayerState* PS = KillerPawn->GetPlayerState<ATioPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
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

void ATioGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ATioPlayerState* PS = Cast<ATioPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player game at this time
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UTioGameplayInterface>())
		{
			continue;
		}

		FActorSaveData SaveData;
		SaveData.ActorName = Actor->GetName();
		SaveData.ActorTransform = Actor->GetTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(SaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's UPROPERTY(SaveGame) variables into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(SaveData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ATioGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UTioSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogGameModeBase, Warning, TEXT("Failed to load game from slot"));
			return;
		}
		UE_LOG(LogGameModeBase, Log, TEXT("Game loaded"));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UTioGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData SaveData : CurrentSaveGame->SavedActors)
			{
				if (Actor->GetName() == SaveData.ActorName)
				{
					Actor->SetActorTransform(SaveData.ActorTransform);

					FMemoryReader MemReader(SaveData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					ITioGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UTioSaveGame>(UGameplayStatics::CreateSaveGameObject(UTioSaveGame::StaticClass()));
		UE_LOG(LogGameModeBase, Log, TEXT("New SaveGame Created"));
	}
}