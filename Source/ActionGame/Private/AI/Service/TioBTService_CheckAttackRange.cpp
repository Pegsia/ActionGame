// Fill out your copyright notice in the Description page of Project Settings.


#include "TioBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

void UTioBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// Check distance and line of sight between Ai and TargetActor
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (TargetActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if (AIPawn)
				{
					float AIDistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinAttackRange = AIDistanceToTarget < 2000.f;
					bool bHasLOS = false;
					if (bWithinAttackRange)
					{
						bHasLOS = AIController->LineOfSightTo(TargetActor);
					}
					BlackBoardComp->SetValueAsBool(WithinAttackRangeKey.SelectedKeyName, (bWithinAttackRange && bHasLOS));
				}
			}
		}
	}
}
