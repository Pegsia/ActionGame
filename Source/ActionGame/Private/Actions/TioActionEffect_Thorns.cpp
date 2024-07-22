// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionEffect_Thorns.h"
#include "TioSystemStatic.h"
#include "TioAttributeComponent.h"
#include <TioGameplayFunctionLibrary.h>

UTioActionEffect_Thorns::UTioActionEffect_Thorns()
{
	ThornsFraction = 0.2f;

	Duration = 0.f;
	Period = 0.f;
}

void UTioActionEffect_Thorns::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(InstigatorActor);
	if (AttributeComp)
	{
		AttributeComp->OnHealthChange.AddDynamic(this, &UTioActionEffect_Thorns::OnHealthChange);
	}
}

void UTioActionEffect_Thorns::StopAction_Implementation(AActor* InstigatorActor)
{
	StopAction_Implementation(InstigatorActor);

	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(InstigatorActor);
	if (AttributeComp)
	{
		AttributeComp->OnHealthChange.RemoveDynamic(this, &UTioActionEffect_Thorns::OnHealthChange);
	}
}

void UTioActionEffect_Thorns::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningActor();
	if (Delta < 0.f && InstigatorActor != OwningActor)
	{
		int DamageAmount = FMath::RoundToInt(Delta * ThornsFraction);
		if (DamageAmount == 0)
		{
			return;
		}

		DamageAmount = FMath::Abs(DamageAmount);
		UTioGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, DamageAmount);
	}
}