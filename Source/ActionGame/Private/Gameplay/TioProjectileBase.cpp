// Fill out your copyright notice in the Description page of Project Settings.


#include "TioProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ATioProjectileBase::ATioProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SpereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(GetRootComponent());

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(GetRootComponent());

	ProMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProMovementComponent");
	ProMovementComponent->bRotationFollowsVelocity = true;
	ProMovementComponent->bInitialVelocityInLocalSpace = true;
	ProMovementComponent->InitialSpeed = 5000;
	ProMovementComponent->ProjectileGravityScale = 0.0f;
}

void ATioProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void ATioProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ATioProjectileBase::OnComponentHit);
}

void ATioProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 16, FColor::Red, false, 10.f, 0, 5.f);
	Explode();
}

void ATioProjectileBase::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

		Destroy();
	}
	
}

