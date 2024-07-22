// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioPowerUpActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONGAME_API ATioPowerUpActor : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ATioPowerUpActor();

	void Interact_Implementation(APawn* Insgitator) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnActor;

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void HideAndCoolDownPowerUp();

	void ShowPowerUpActor();

	void SetPowerUpState(bool bNewIsActive);
	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
};
