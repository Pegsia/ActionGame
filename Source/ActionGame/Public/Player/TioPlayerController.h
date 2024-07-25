// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TioPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class ACTIONGAME_API ATioPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATioPlayerController();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDClass;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

	//~ Begin APlayerController Interface
	virtual void BeginPlay() override;
	/* Called when player controller is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay
	(esp. in multiplayer clients where not all data such as PlayerState may have been received yet) */
	virtual void BeginPlayingState() override;
	void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;
	void OnRep_PlayerState() override; // 另一个解决PlayerState的方法
	//~ End APlayerController Interface
};
