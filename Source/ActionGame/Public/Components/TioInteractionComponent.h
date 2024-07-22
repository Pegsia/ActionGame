// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TioInteractionComponent.generated.h"

class UTioWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UTioInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY()
	AActor* FocusActor;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UTioWorldUserWidget> InteractWidgetClass;

	UPROPERTY()
	UTioWorldUserWidget* InteractWidgetInstance;
	
	void FindBestInteractable();

	void PrimaryInteract();

	UFUNCTION(Server, Reliable, Category = "Interact")
	void ServerInteract(AActor* InFocusActor);

	UTioInteractionComponent();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
