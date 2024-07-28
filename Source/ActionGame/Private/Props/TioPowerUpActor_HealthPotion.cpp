// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerUpActor_HealthPotion.h"
#include "TioAttributeComponent.h"
#include "System/TioSystemStatic.h"
#include "TioPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ATioPowerUpActor_HealthPotion::ATioPowerUpActor_HealthPotion()
{
	HealAmount = 40.f;
	CreditsCost = 30;
}

void ATioPowerUpActor_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(InstigatorPawn);
	if (AttributeComp && !AttributeComp->IsFullHealth())
	{
		ATioPlayerState* PS = InstigatorPawn->GetPlayerState<ATioPlayerState>();
		if (PS && PS->RemoveCredits(CreditsCost))
		{
			if (AttributeComp->ApplyHealthChange(this, HealAmount))
			{
				HideAndCoolDownPowerUp();
			}
		}
	}
}

FText ATioPowerUpActor_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		// NSLOCOTEXT("InteractableActors", "HealthPotion_FullHealth", "Already at full health.")
		return LOCTEXT("HealthPotion_FullHealth", "Already at full health.");
	}
	return FText::Format(LOCTEXT("HealthPotion_CreaditsCost", "Cost {0} Credits to resotre {1} health."), CreditsCost, HealAmount);
}

#undef LOCTEXT_NAMESPACE