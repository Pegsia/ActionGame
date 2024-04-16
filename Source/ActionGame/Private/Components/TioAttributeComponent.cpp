// Fill out your copyright notice in the Description page of Project Settings.

#include "TioAttributeComponent.h"

UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	return true;
}
