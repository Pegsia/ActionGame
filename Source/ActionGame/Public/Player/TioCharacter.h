// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTioInteractionComponent;
class UTioAttributeComponent;

UCLASS()
class ACTIONGAME_API ATioCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> TelePortClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> BlackHoleClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float TraceDistance;

	FTimerHandle TimerHandle_AttackDelay;
	FTimerHandle TimerHandle_TeleportDelay;
	FTimerHandle TimerHandle_BlackHoleDelay;

public:
	ATioCharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTioInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioAttributeComponent* AttributeComponent;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

	void TelePort();

	void TelePort_TimeElapsed();

	void BlackHole();

	void BlackHole_TimeElapsed();

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
