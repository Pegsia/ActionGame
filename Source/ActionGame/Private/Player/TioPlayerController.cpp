// Fill out your copyright notice in the Description page of Project Settings.


#include "TioPlayerController.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogTioPlayerController, All, All);

ATioPlayerController::ATioPlayerController()
{

}

void ATioPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATioPlayerController::BeginPlayingState()
{
	SetInputMode(FInputModeGameOnly());
	if (IsLocalController() && MainHUDClass)
	{
		UUserWidget* MainHUD = CreateWidget<UUserWidget>(this, MainHUDClass);
		MainHUD->AddToViewport();
	}
}

void ATioPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ATioPlayerController::TogglePauseMenu);
}

void ATioPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	if (PauseMenuClass)
	{
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
		if (PauseMenuInstance)
		{
			PauseMenuInstance->AddToViewport(100); // Above All

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		UE_LOG(LogTioPlayerController, Error, TEXT("No Pause Ment Class Assigned in ATioPlayerController!"));
	}
}

void ATioPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateChanged.Broadcast(PlayerState);// Can Bind OnPlayerStateChanged in Credits Widget
}

void ATioPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn); 
}
