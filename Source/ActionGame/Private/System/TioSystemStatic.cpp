// Fill out your copyright notice in the Description page of Project Settings.


#include "TioSystemStatic.h"
#include "TioAttributeComponent.h"

UTioAttributeComponent* UTioSystemStatic::GetAttributeComponent(AActor* TargetActor)
{
	if (TargetActor)
	{
		return Cast<UTioAttributeComponent>(TargetActor->GetComponentByClass(UTioAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UTioSystemStatic::IsAlive(AActor* TargetActor)
{
	
	UTioAttributeComponent* AttributeComp = GetAttributeComponent(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}	
	return false;
}
