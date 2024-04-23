// Fill out your copyright notice in the Description page of Project Settings.
#include "TioAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATioAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr, assign in your AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}

}
