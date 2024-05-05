// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "System/TioSystemStatic.h"

ATioMagicProjectile::ATioMagicProjectile()
{
	DamageAmount = 20.f;
}

void ATioMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 防止没有撞击，也没有overlap造成不会销毁
	SetLifeSpan(4.f);
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
		UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(OtherActor);
		if (AttributeComp)
		{
			if (AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount))
			{
				// 只有造成伤害才会爆炸
				Explode();
			}
		}	
	}
}

