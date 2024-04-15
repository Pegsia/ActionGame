// Fill out your copyright notice in the Description page of Project Settings.


#include "TioDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATioDashProjectile::ATioDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;
}

void ATioDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Detonate, this, &ATioDashProjectile::Explode, DetonateDelay);
}

void ATioDashProjectile::Explode_Implementation()
{
	// 如果先Hit了，移除Timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Detonate);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComponent->DeactivateSystem();

	ProMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_Teleport;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Teleport, this, &ATioDashProjectile::Teleport, TeleportDelay);
}

void ATioDashProjectile::Teleport()
{
	AActor* InsigatorActor = GetInstigator();
	if (InsigatorActor)
	{
		InsigatorActor->TeleportTo(GetActorLocation(), InsigatorActor->GetActorRotation(), false, false);
	}
	Destroy();
}

