// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPlayerController.h"
#include "Blueprint/UserWidget.h"

ATioPlayerController::ATioPlayerController()
{

}

void ATioPlayerController::BeginPlay()
{
	if (IsLocalController() && MainHUDClass)
	{
		UUserWidget* MainHUD = CreateWidget<UUserWidget>(this, MainHUDClass);
		MainHUD->AddToViewport();
	}
}

void ATioPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}
