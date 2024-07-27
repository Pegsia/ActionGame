// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAction.h"
#include "TioActionComponent.h"
#include "../ActionGame.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogAction, All, All);

UTioAction::UTioAction()
{
	bAutoStart = false;
}

void UTioAction::Initialize(UTioActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

bool UTioAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (IsRunning())
	{
		/* 一个相同的动作结束之前不可再运行
		*  防止像Projectiole类的带timer方法反复调用StopAction
		* */
		return false;
	}

	UTioActionComponent* OwningComp = GetOwningComponent();
	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UTioAction::StartAction_Implementation(AActor* InstigatorActor)
{
	//UE_LOG(LogAction, Log, TEXT("Start Action %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantsTags);
	OwningComp->OnActionStarted.Broadcast(OwningComp, this); // for EffectSlot_Widget
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		StartTime = GetWorld()->GetTimeSeconds();
	}	

	RepData.bIsRunning = true;
	RepData.InstigatorActor = InstigatorActor;
}

void UTioAction::StopAction_Implementation(AActor* InstigatorActor)
{
	//UE_LOG(LogAction, Log, TEXT("Stop Action %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning); OnRep之后，StopAction必须是false

	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantsTags);
	OwningComp->OnActionStopped.Broadcast(OwningComp, this);

	RepData.bIsRunning = false;
	RepData.InstigatorActor = InstigatorActor;
}

UWorld* UTioAction::GetWorld() const
{
	// set in TioActionComponent.cpp AddAction
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool UTioAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UTioAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.InstigatorActor);
	}
	else
	{
		StopAction(RepData.InstigatorActor);
	}
}

UTioActionComponent* UTioAction::GetOwningComponent() const
{
	return ActionComponent;
}

AActor* UTioAction::GetOwningActor() const
{
	return ActionComponent->GetOwner();
}

void UTioAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTioAction, RepData);
	DOREPLIFETIME(UTioAction, StartTime);
	DOREPLIFETIME(UTioAction, ActionComponent);
}