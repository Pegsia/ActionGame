// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "TioAction.generated.h"

class UWorld;

UCLASS(Blueprintable)
class ACTIONGAME_API UTioAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;

public:

	UTioAction();

	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	// 类似GameplayTag负责对比名称
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTioActionComponent* GetOwningComponent() const;

	UWorld* GetWorld() const override;
};
