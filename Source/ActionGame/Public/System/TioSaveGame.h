// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TioSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	/* For variables with UPROPERTY(SaveGame), convert them to binary data and save them in ByteData array */
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class ACTIONGAME_API UTioSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
