// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/TioProjectileBase.h"
#include "TioBlackholeProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONGAME_API ATioBlackholeProjectile : public ATioProjectileBase
{
	GENERATED_BODY()
public:

	ATioBlackholeProjectile();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	URadialForceComponent* ForceComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};
