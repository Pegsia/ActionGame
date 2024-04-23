// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ATioPowerUpActor::ATioPowerUpActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUpActor");
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(GetRootComponent());

	RespawnTime = 10.f;
}

void ATioPowerUpActor::Interact_Implementation(APawn* Insgitator)
{

}

void ATioPowerUpActor::HideAndCoolDownPowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnActor, this, &ATioPowerUpActor::ShowPowerUpActor, RespawnTime);
}

void ATioPowerUpActor::ShowPowerUpActor()
{
	SetPowerUpState(true);
}

void ATioPowerUpActor::SetPowerUpState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}
