// Fill out your copyright notice in the Description page of Project Settings.


#include "TioItemChest.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"

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
	bLidOpended = false;
	InitLidmeshState = true;

	SetReplicates(true);
}

void ATioItemChest::BeginPlay()
{
	Super::BeginPlay();
}

void ATioItemChest::OnActorLoaded_Implementation()
{
	// Only On Server
	InitLidmeshState = false;
	float NextPitch = bLidOpended ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(NextPitch, 0.f, 0.f));
}

void ATioItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpended = !bLidOpended;
	OnRep_LidOpened();
}

void ATioItemChest::OnRep_LidOpened()
{
	ChangeState();
}

void ATioItemChest::ChangeState_Implementation()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ChangeState"));*/
}

void ATioItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATioItemChest, bLidOpended);
}