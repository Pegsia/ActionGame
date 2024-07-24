// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChange, AActor*, InstigatorActor, UTioAttributeComponent*, AttributeComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UTioAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Rage")
	FOnAttributeChange OnRageChange;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	
	UFUNCTION(BlueprintCallable)
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UTioAttributeComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const { return HealthMax; }

	UFUNCTION(BlueprintCallable)
	float GetRage() const { return Rage; }

protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Health")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Rage")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Rage")
	float RageMax;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastOnHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastOnRageChanged(AActor* InstigatorActor, float NewRage, float Delta);
};
