// Fill out your copyright notice in the Description page of Project Settings.


#include "TioBlackholeProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATioBlackholeProjectile::ATioBlackholeProjectile()
{
	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComponent->SetupAttachment(GetRootComponent());
	ForceComponent->ForceStrength = -400000.f;
	ForceComponent->Radius = 700.f;
	ForceComponent->Falloff = ERadialImpulseFalloff::RIF_Constant;

	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	// 撞墙消失
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	ProMovementComponent->InitialSpeed = 800.f;
}

void ATioBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	// 谁也没撞到就10秒后自己消亡
	SetLifeSpan(5.f);
}

void ATioBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATioBlackholeProjectile::OnBeginOverlap);
}

void ATioBlackholeProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherActor->SetLifeSpan(3.f);
	}
}

