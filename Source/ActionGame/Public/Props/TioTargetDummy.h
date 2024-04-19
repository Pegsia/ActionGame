// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioTargetDummy.generated.h"

UCLASS()
class ACTIONGAME_API ATioTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	ATioTargetDummy();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTioAttributeComponent* AttributeComponent;

};
