// Fill out your copyright notice in the Description page of Project Settings.

#include "TioAttributeComponent.h"

UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(OldHealth + Delta, 0.f, HealthMax);
	
	OnHealthChange.Broadcast(nullptr, this, Health, Health - OldHealth);

	return true;
}

bool UTioAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}
