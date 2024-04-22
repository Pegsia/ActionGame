// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "TioBTService_CheckAttackRange.generated.h"

UCLASS()
class ACTIONGAME_API UTioBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector WithinAttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
