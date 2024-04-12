// Fill out your copyright notice in the Description page of Project Settings.


#include "TioItemChest.h"

ATioItemChest::ATioItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BashMesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(GetRootComponent());

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

