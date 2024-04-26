// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioAICharacter.generated.h"

class UPawnSensingComponent;
class UTioAttributeComponent;

UCLASS()
class ACTIONGAME_API ATioAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	bool IsAlive() const;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta);

	ATioAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTioAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void SetTargetActor(AActor* TargetActor);

	virtual void PostInitializeComponents() override;

};
