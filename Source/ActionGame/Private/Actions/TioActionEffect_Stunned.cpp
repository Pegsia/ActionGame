// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionEffect_Stunned.h"
#include "GameFramework/Character.h"

void UTioActionEffect_Stunned::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController());
		if (PC)
		{
			OwningCharacter->DisableInput(PC);
		}
	}
}

void UTioActionEffect_Stunned::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController());
		if (PC)
		{
			OwningCharacter->EnableInput(PC);
		}
	}
}
