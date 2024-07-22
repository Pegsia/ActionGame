// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionComponent.h"
#include "Actions/TioAction.h"
#include "../ActionGame.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

DEFINE_LOG_CATEGORY_STATIC(LogActionComp, All, All);
static TAutoConsoleVariable<bool> CVarShowGameplayTag(TEXT("Tio.ShowGameplayTag"), false, TEXT("ShowGamplayTag in ActionComp"), ECVF_Cheat);

UTioActionComponent::UTioActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTioActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UTioAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UTioActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CVarShowGameplayTag.GetValueOnGameThread())
	{
		const ENetRole Role = GetOwner()->GetLocalRole();
		FString RoleString = Role == ROLE_Authority ? TEXT("Server") : TEXT("Client");
		FString DebugMes = FString::Printf(TEXT("[%s] [%s] Active Tags: %s"), *RoleString, *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMes);
	}

	for (UTioAction* Action : ActionArray)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString Msg = FString::Printf(TEXT("[%s] Action %s: Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			*GetNameSafe(Action->GetOuter()));
		LogOnScreen(this, Msg, TextColor, 0.0f);
	}
	
}

void UTioActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<UTioAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	// Only on Server
	UTioAction* NewAction = NewObject<UTioAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
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

			if (!GetOwner()->HasAuthority()) // Client
			{
				ServerStartAction(InstigatorActor, ActionName);
			}

			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

void UTioActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
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

bool UTioActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UTioAction* Action : ActionArray)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}


void UTioActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTioActionComponent, ActionArray);
}