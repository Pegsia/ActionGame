// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TioAICharacter.generated.h"

class UPawnSensingComponent;
class UTioAttributeComponent;
class UTioWorldUserWidget;
class UUserWidget;
class UTioActionComponent;

UCLASS()
class ACTIONGAME_API ATioAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool ApplyHealth(AActor* InstigatorActor, float Amount = 100.f);

	bool IsAlive() const;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta);

	ATioAICharacter();

protected:

	UTioWorldUserWidget* HealthWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTioAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTioActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	FName ParamName_TimeToHit;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void SetTargetActor(AActor* TargetActor);

	virtual void PostInitializeComponents() override;

};
