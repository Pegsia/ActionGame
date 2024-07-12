// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioProjectileBase.h"
#include "GameplayTagContainer.h"
#include "TioMagicProjectile.generated.h"

class UTioAction_Effect;

UCLASS()
class ACTIONGAME_API ATioMagicProjectile : public ATioProjectileBase
{
	GENERATED_BODY()
	
public:	
	ATioMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UTioAction_Effect> BurnningEffectClass;

	bool bParried;

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};
