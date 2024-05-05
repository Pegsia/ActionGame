// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TioBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UTioBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector KSTargetActor;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName HandSocket;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UTioBTTask_RangedAttack();
};
