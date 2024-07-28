// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAnimInstance.h"
#include "TioActionComponent.h"

void UTioAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UTioActionComponent>(OwningActor->GetComponentByClass(UTioActionComponent::StaticClass()));
	}
	
}

void UTioAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
