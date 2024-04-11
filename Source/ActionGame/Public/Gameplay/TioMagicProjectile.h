// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONGAME_API ATioMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATioMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	UProjectileMovementComponent* ProMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	UParticleSystemComponent* EffectComponent;

	virtual void BeginPlay() override;


};
