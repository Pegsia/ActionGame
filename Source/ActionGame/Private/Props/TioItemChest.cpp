// Fill out your copyright notice in the Description page of Project Settings.


#include "TioItemChest.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"

ATioItemChest::ATioItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BashMesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(GetRootComponent());

	Gold = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
	Gold->SetupAttachment(GetRootComponent());

	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(Gold);
	Effect->bAutoActivate = false;

	TargetPitch = 110.f;
}

void ATioItemChest::BeginPlay()
{
	Super::BeginPlay();
}


void ATioItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

