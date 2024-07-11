// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTioInteractionComponent;
class UTioAttributeComponent;
class UTioActionComponent;

UCLASS()
class ACTIONGAME_API ATioCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	FName ParamName_TimeToHit;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioActionComponent* ActionComponent;

	// Move
	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartSprint();
	void StopSprint();

	// Projectiles
	void PrimaryAttack();
	void TelePort();
	void BlackHole();

	// Interact
	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
