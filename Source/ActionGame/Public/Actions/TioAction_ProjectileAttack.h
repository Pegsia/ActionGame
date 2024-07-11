// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/TioAction.h"
#include "TioAction_ProjectileAttack.generated.h"


UCLASS()
class ACTIONGAME_API UTioAction_ProjectileAttack : public UTioAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float TraceDistance;

	UFUNCTION()
	void PrimaryAttack_TimeElapsed(ACharacter* InstigatorCharacter);

public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	UTioAction_ProjectileAttack();
};
