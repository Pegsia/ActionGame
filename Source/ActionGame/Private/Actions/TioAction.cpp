// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAction.h"
#include "TioActionComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAction, All, All);

UTioAction::UTioAction()
{
	bAutoStart = false;
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
	UE_LOG(LogAction, Log, TEXT("Start Action %s"), *GetNameSafe(this));

	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UTioAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogAction, Log, TEXT("Stop Action %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UTioActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UTioAction::GetWorld() const
{
	// set in TioActionComponent.cpp AddAction
	UActorComponent* OuterComp = GetOwningComponent();
	if (OuterComp)
	{
		return OuterComp->GetWorld();
	}
	return nullptr;
}

bool UTioAction::IsRunning() const
{
	return bIsRunning;
}

UTioActionComponent* UTioAction::GetOwningComponent() const
{
	return Cast<UTioActionComponent>(GetOuter());
}

