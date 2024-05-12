// Fill out your copyright notice in the Description page of Project Settings.


#include "TioGameplayFunctionLibrary.h"
#include "TioAttributeComponent.h"
#include "TioSystemStatic.h"

static TAutoConsoleVariable<float> CvarImpulseStrength(TEXT("Tio.ImpulseStrength"), 100000.f, TEXT("Impulse Strength in Apply Direction Damage(GameplayFunction)"));

bool UTioGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamgeAmount)
{
	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamgeAmount);
	}
	return false;
}

bool UTioGameplayFunctionLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamgeAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamgeAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			float ImpulseStrength = CvarImpulseStrength.GetValueOnGameThread();
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * ImpulseStrength, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
