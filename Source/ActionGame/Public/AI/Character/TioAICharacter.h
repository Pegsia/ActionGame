// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONGAME_API ATioAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATioAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	virtual void PostInitializeComponents() override;

};
