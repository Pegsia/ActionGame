// Fill out your copyright notice in the Description page of Project Settings.


#include "TioBTService_CheckHealth.h"
#include "AIController.h"
#include "TioAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TioSystemStatic.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

UTioBTService_CheckHealth::UTioBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void UTioBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ensure(MyPawn))
	{
		UTioAttributeComponent* AttributeComp = UTioSystemStatic::GetAttributeComponent(MyPawn);
		if(ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) < LowHealthFraction;
			BBComp->SetValueAsBool(bLowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
