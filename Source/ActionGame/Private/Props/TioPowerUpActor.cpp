// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPowerUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ATioPowerUpActor::ATioPowerUpActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUpActor");
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(GetRootComponent());

	RespawnTime = 10.f;
	
	bIsActive = true;
	// Directly set bool instead of going through SetReplicates(true) within constructor,
	// Only use SetReplicates() outside constructor
	bReplicates = true;
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

void ATioPowerUpActor::SetPowerUpState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ATioPowerUpActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ATioPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

FText ATioPowerUpActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ATioPowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATioPowerUpActor, bIsActive);
}
