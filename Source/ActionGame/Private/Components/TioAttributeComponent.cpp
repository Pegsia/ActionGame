// Fill out your copyright notice in the Description page of Project Settings.

#include "TioAttributeComponent.h"

UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(OldHealth + Delta, 0.f, HealthMax);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool UTioAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool UTioAttributeComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Health, HealthMax);
}
