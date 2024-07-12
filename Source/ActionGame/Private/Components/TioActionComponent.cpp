// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionComponent.h"
#include "Actions/TioAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogActionComp, All, All);

UTioActionComponent::UTioActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTioActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UTioAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UTioActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMes = FString::Printf(TEXT("[%s] Active Tags: %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMes);
}

void UTioActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<UTioAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTioAction* NewAction = NewObject<UTioAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		ActionArray.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(InstigatorActor))) //保险
		{
			NewAction->StartAction(InstigatorActor);
		}
	}
}

void UTioActionComponent::RemoveAction(AActor* InstigatorActor, UTioAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		UE_LOG(LogActionComp, Error, TEXT("ActionToRemove is Missing or Running"));
		return;
	}
	ActionArray.Remove(ActionToRemove);
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