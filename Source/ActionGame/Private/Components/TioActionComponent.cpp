// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionComponent.h"
#include "Actions/TioAction.h"



UTioActionComponent::UTioActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTioActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UTioAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UTioActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMes = FString::Printf(TEXT("[%s] Active Tags: %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMes);
}

void UTioActionComponent::AddAction(TSubclassOf<UTioAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTioAction* NewAction = NewObject<UTioAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		ActionArray.Add(NewAction);
	}
}

bool UTioActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (UTioAction* Action : ActionArray)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(InstigatorActor))
			{
				FString ActionMsg = FString::Printf(TEXT("[%s] Faile to run %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, ActionMsg);
				continue;
			}
			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

bool UTioActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (UTioAction* Action : ActionArray)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(InstigatorActor);
				return true;
			}
		}
	}
	return false;
}