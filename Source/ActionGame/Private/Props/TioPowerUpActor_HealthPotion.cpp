// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerUpActor_HealthPotion.h"
#include "TioAttributeComponent.h"
#include "System/TioSystemStatic.h"

ATioPowerUpActor_HealthPotion::ATioPowerUpActor_HealthPotion()
{
	HealAmount = 40.f;
}

void ATioPowerUpActor_HealthPotion::Interact_Implementation(APawn* InsgitatorPawn)
{
	if (!ensure(InsgitatorPawn))
	{
		return;
	}

	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(InsgitatorPawn);
	if (AttributeComp && !AttributeComp->IsFullHealth())
	{
		if (AttributeComp->ApplyHealthChange(this, HealAmount))
		{
			HideAndCoolDownPowerUp();
		}
	}
}
