// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundCue;

UCLASS(ABSTRACT) //Keep this out of certain dropdown windows like SpawnActor in Unreal Editor 
class ACTIONGAME_API ATioProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATioProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
	UProjectileMovementComponent* ProMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
	UParticleSystemComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Comp")
	UParticleSystem* ImpactVFX;
	

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* ImpactSound;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// ComponentHit后Explode，在子类中重写
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();

	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
};
