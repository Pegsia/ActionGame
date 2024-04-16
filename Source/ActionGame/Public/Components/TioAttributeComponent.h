// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UTioAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTioAttributeComponent();

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(float Delta);

protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthMax;
};
