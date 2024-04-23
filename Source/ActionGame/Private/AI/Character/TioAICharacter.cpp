// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

ATioAICharacter::ATioAICharacter()
{
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void ATioAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATioAICharacter::OnSeePawn);
}

void ATioAICharacter::OnSeePawn(APawn* Pawn)
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
        BBComp->SetValueAsObject("TargetActor", Pawn);

        //DrawDebugString(GetWorld(), GetActorLocation(), "On See Pawn", nullptr, FColor::Blue, 4.f, false);
    }
}
