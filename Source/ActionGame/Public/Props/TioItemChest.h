// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioGameplayInterface.h"
#include "TioItemChest.generated.h"

UCLASS()
class ACTIONGAME_API ATioItemChest : public AActor, public ITioGameplayInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Interact")
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);

	ATioItemChest();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* LidMesh;	

	virtual void BeginPlay() override;

};
