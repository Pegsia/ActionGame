// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TioInteractionComponent.h"
#include "TioGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Widget/TioWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(TEXT("Tio.DrawDebugInteraction"), false, TEXT("Enable Debuge Line for Interact Comp"), ECVF_Cheat);

UTioInteractionComponent::UTioInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 800.f;
	TraceRadius = 30.f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
}

void UTioInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTioInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UTioInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDrawDebugInteraction.GetValueOnGameThread();

	TArray<FHitResult> Hits;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	FColor HitColor = bBlockingHit ? FColor::Red : FColor::Green;

	FocusActor = nullptr; // clear before set

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, HitColor, false, 0.f, 0, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTioGameplayInterface>())
			{
				FocusActor = HitActor;
				break;
			}
		}
	}

	if (FocusActor) // 设置widget
	{
		if (InteractWidgetInstance == nullptr && ensure(InteractWidgetClass))
		{
			InteractWidgetInstance = CreateWidget<UTioWorldUserWidget>(GetWorld(), InteractWidgetClass);
		}
		if (InteractWidgetInstance)
		{
			InteractWidgetInstance->AttachActor = FocusActor;
			if (!InteractWidgetInstance->IsInViewport())
			{
				InteractWidgetInstance->AddToViewport();
			}	
		}
	}
	else
	{
		if (InteractWidgetInstance) // @fixme:nullptr问题没有解决
		{
			// 怀疑是快速移动，造成Widget回收的时候再次访问，已经不可的问题
			InteractWidgetInstance->RemoveFromParent();
			InteractWidgetInstance = nullptr; // 添加
		}		
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, HitColor, false, 0.0f, 0, 2.0f);
	}
}

void UTioInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}

void UTioInteractionComponent::ServerInteract_Implementation(AActor* InFocusActor)
{
	if (InFocusActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("No focus actor to interact")));
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ITioGameplayInterface::Execute_Interact(InFocusActor, MyPawn);
}

