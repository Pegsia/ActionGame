// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ATioMagicProjectile::ATioMagicProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SpereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(GetRootComponent());

	ProMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProMovementComponent");
	ProMovementComponent->bRotationFollowsVelocity = true;
	ProMovementComponent->bInitialVelocityInLocalSpace = true;
	ProMovementComponent->InitialSpeed = 5000;
	ProMovementComponent->ProjectileGravityScale = 0.0f;
}

void ATioMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


