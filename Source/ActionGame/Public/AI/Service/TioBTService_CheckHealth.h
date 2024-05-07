// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TioBTService_CheckHealth.generated.h"

UCLASS()
class ACTIONGAME_API UTioBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Health")
	float LowHealthFraction;

	UPROPERTY(EditAnywhere, Category = "Health")
	FBlackboardKeySelector bLowHealthKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UTioBTService_CheckHealth();
};
