// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TioInteractionComponent.h"
#include "TioGameplayInterface.h"
#include "DrawDebugHelpers.h"

UTioInteractionComponent::UTioInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 500.f;
	TraceRadius = 30.f;
}


void UTioInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UTioInteractionComponent::PrimaryInteract()
{
	TArray<FHitResult> Hits;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	FColor HitColor = bBlockingHit ? FColor::Red : FColor::Green;

	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, HitColor, false, 2.0f, 0, 2.0f);

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTioGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ITioGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, HitColor, false, 2.0f, 0, 2.0f);
}


