// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioItemChest.generated.h"

class UParticleSystemComponent;
class UTimelineComponent;

UCLASS()
class ACTIONGAME_API ATioItemChest : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Interact")
	float TargetPitch;

	//~ Begin ITioGameplayInterface
	void OnActorLoaded_Implementation();
	void Interact_Implementation(APawn* InstigatorPawn);	
	//~ End ITioGameplayInterface
	ATioItemChest();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Init")
	bool InitLidmeshState;

	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly, SaveGame, Category = "Net")
	bool bLidOpended;

	UFUNCTION()
	void OnRep_LidOpened();

	UFUNCTION(BlueprintNativeEvent)
	void ChangeState();

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* Gold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UParticleSystemComponent* Effect;

	virtual void BeginPlay() override;

};
