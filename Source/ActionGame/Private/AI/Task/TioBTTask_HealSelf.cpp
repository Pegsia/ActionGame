// Fill out your copyright notice in the Description page of Project Settings.


#include "TioBTTask_HealSelf.h"
#include "AIController.h"
#include "TioAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TioSystemStatic.h"

EBTNodeResult::Type UTioBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}
	return EBTNodeResult::Succeeded;
}
