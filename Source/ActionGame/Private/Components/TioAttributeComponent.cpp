// Fill out your copyright notice in the Description page of Project Settings.

#include "TioAttributeComponent.h"
#include "TioGameModeBase.h"

static TAutoConsoleVariable<float> CvarDamageMultiplier(TEXT("Tio.DamageMultiplier"), 1.f, TEXT("Global Damage Multiplier for Attribute Comp"), ECVF_Cheat);

UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;
}

bool UTioAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false; 
	}

	if (Delta < 0.f)
	{
		float DamageMultiplier = CvarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(OldHealth + Delta, 0.f, HealthMax);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);

	//Died
	if (ActualDelta < 0.f && Health == 0.f)
	{
		ATioGameModeBase* GM = GetWorld()->GetAuthGameMode<ATioGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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

bool UTioAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

