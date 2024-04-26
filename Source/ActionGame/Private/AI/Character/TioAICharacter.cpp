// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "BrainComponent.h"

ATioAICharacter::ATioAICharacter()
{
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
    AttributeComponent = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComponent");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATioAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATioAICharacter::OnSeePawn);
    AttributeComponent->OnHealthChange.AddDynamic(this, &ATioAICharacter::OnHealthChange);
}

void ATioAICharacter::OnSeePawn(APawn* Pawn)
{
	SetTargetActor(Pawn);
    //DrawDebugString(GetWorld(), GetActorLocation(), "On See Pawn", nullptr, FColor::Blue, 4.f, false);
}

void ATioAICharacter::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
    if (Delta < 0.f)
    {
        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (NewHealth <= 0.f)
        {
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }

            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");

            SetLifeSpan(10.f);
        }
    }
}

void ATioAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
        AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}

bool ATioAICharacter::IsAlive() const
{
    return AttributeComponent->IsAlive();
}

