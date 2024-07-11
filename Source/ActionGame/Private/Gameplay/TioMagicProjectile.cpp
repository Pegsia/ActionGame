// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TioMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "System/TioSystemStatic.h"
#include "TioGameplayFunctionLibrary.h"
#include "TioActionComponent.h"

ATioMagicProjectile::ATioMagicProjectile()
{
	DamageAmount = 20.f;
	bParried = false; // 防止反复反弹
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
		if (!bParried)
		{
			UTioActionComponent* ActionComp = Cast<UTioActionComponent>(OtherActor->GetComponentByClass(UTioActionComponent::StaticClass()));
			if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
			{
				ProMovementComponent->Velocity = -ProMovementComponent->Velocity;
				SetInstigator(Cast<APawn>(OtherActor));
			}
			bParried = true;
			return; // 否则直接爆炸了
		}

		if (UTioGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			// 只有造成伤害才会爆炸
			Explode();
		}
	}
}

