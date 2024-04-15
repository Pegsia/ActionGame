// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TioProjectileBase.h"
#include "TioMagicProjectile.generated.h"

UCLASS()
class ACTIONGAME_API ATioMagicProjectile : public ATioProjectileBase
{
	GENERATED_BODY()
	
public:	
	ATioMagicProjectile();

protected:

	virtual void BeginPlay() override;

};
