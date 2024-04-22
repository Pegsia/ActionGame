// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"

ATioMagicProjectile::ATioMagicProjectile()
{
	DamageAmount = 20.f;
}

void ATioMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATioMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATioMagicProjectile::OnComponentOverlap);
}

void ATioMagicProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UTioAttributeComponent* AttributeComp = Cast<UTioAttributeComponent>(OtherActor->GetComponentByClass(UTioAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-DamageAmount);
		}
	}
	Explode();
}

