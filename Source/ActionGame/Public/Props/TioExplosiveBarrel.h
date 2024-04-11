// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONGAME_API ATioExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ATioExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	URadialForceComponent* RForceComponent;
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void PostInitializeComponents() override;


};
