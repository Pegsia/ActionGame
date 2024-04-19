// Fill out your copyright notice in the Description page of Project Settings.


#include "TioTargetDummy.h"
#include "TioAttributeComponent.h"

ATioTargetDummy::ATioTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	AttributeComponent = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComponent");
}

void ATioTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this, &ATioTargetDummy::OnHealthChange);
}

void ATioTargetDummy::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}


