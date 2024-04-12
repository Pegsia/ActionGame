// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UTioInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceRadius;

	void PrimaryInteract();

	UTioInteractionComponent();

protected:
	virtual void BeginPlay() override;

};
