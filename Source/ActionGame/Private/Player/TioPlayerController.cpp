// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPlayerController.h"
#include "Blueprint/UserWidget.h"

ATioPlayerController::ATioPlayerController()
{

}

void ATioPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATioPlayerController::BeginPlayingState()
{
	if (IsLocalController() && MainHUDClass)
	{
		UUserWidget* MainHUD = CreateWidget<UUserWidget>(this, MainHUDClass);
		MainHUD->AddToViewport();
	}
}

void ATioPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ATioPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}
