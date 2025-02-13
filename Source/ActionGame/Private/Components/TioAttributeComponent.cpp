// Fill out your copyright notice in the Description page of Project Settings.

#include "TioAttributeComponent.h"
#include "TioGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CvarDamageMultiplier(TEXT("Tio.DamageMultiplier"), 1.f, TEXT("Global Damage Multiplier for Attribute Comp"), ECVF_Cheat);

UTioAttributeComponent::UTioAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;

	Rage = 0.f;
	RageMax = 100.f;

	SetIsReplicatedByDefault(true);
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

	// 允许客户端计算血量变化，但不允许更改，这样可以返回正确的击打以及回复结果
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(OldHealth + Delta, 0.f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			NetMulticastOnHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		//Died
		if (ActualDelta < 0.f && Health == 0.f)
		{
			ATioGameModeBase* GM = GetWorld()->GetAuthGameMode<ATioGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

void UTioAttributeComponent::NetMulticastOnHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChange.Broadcast(InstigatorActor, this, Health, Delta);
}

bool UTioAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	float NewRage = FMath::Clamp(Rage + Delta, 0.f, RageMax);
	float ActualDelta = NewRage - OldRage;
	
	if (GetOwner()->HasAuthority())
	{
		Rage = NewRage;
		if (ActualDelta != 0)
		{
			NetMulticastOnRageChanged(InstigatorActor, Rage, ActualDelta);
		}
	}
	
	return ActualDelta != 0;
}

void UTioAttributeComponent::NetMulticastOnRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
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

void UTioAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTioAttributeComponent, Health);
	DOREPLIFETIME(UTioAttributeComponent, HealthMax);

	DOREPLIFETIME(UTioAttributeComponent, Rage);
	DOREPLIFETIME(UTioAttributeComponent, RageMax);
}