// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChange, AActor*, InstigatorActor, UTioAttributeComponent*, AttributeComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UTioAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTioAttributeComponent();

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnAttributeChange OnHealthChange;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthMax;
};
