// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TioSystemStatic.generated.h"

UCLASS()
class ACTIONGAME_API UTioSystemStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static UTioAttributeComponent* GetAttributeComponent(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static bool IsAlive(AActor* TargetActor);
};
