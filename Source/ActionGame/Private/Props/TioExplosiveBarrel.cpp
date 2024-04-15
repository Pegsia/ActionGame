// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/TioExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

ATioExplosiveBarrel::ATioExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	SetRootComponent(MeshComponent);

	RForceComponent = CreateDefaultSubobject<URadialForceComponent>("RForceComponent");
	RForceComponent->SetupAttachment(GetRootComponent());
	RForceComponent->SetAutoActivate(false);

	RForceComponent->Radius = 650.0f;
	RForceComponent->ForceStrength = 1000.0f;
	RForceComponent->bImpulseVelChange = true;

	RForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ATioExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &ATioExplosiveBarrel::OnActorHit);
}


void ATioExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("%s hit the explosivebarrel"), *GetNameSafe(OtherActor));

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Blue, 2.0f, true);
}

