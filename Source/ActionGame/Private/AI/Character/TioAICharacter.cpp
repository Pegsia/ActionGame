// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "BrainComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widget/TioWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TioActionComponent.h"

ATioAICharacter::ATioAICharacter()
{
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
    AttributeComponent = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComponent");
    ActionComponent = CreateDefaultSubobject<UTioActionComponent>("ActionComponent");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    ParamName_TimeToHit = "TimeToHit";
    TargetActorKey = "TargetActor";
}

void ATioAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATioAICharacter::OnSeePawn);
    AttributeComponent->OnHealthChange.AddDynamic(this, &ATioAICharacter::OnHealthChange);
}

void ATioAICharacter::OnSeePawn(APawn* Pawn)
{
    if (GetTargetActor() != Pawn)
    {
        SetTargetActor(Pawn);
        MulticastPawnSeen();
    }
    //DrawDebugString(GetWorld(), GetActorLocation(), "On See Pawn", nullptr, FColor::Blue, 4.f, false);
}

void ATioAICharacter::MulticastPawnSeen_Implementation()
{
	UTioWorldUserWidget* SpottedWidget = CreateWidget<UTioWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (SpottedWidget)
	{
		SpottedWidget->AttachActor = this;
		SpottedWidget->AddToViewport(10); // 确保其在别的UI之上
	}
}

void ATioAICharacter::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
    if (Delta < 0.f)
    {
        if (InstigatorActor != this)
        {
			if (InstigatorActor->HasAuthority() && InstigatorActor != GetTargetActor())
            //if (InstigatorActor != GetTargetActor())
			{
				MulticastPawnSeen();
			}
            SetTargetActor(InstigatorActor);
        }

        if (HealthWidget == nullptr)
        {
            HealthWidget = CreateWidget<UTioWorldUserWidget>(GetWorld(), HealthWidgetClass);
            if (HealthWidget)
            {
                HealthWidget->AttachActor = this;
                HealthWidget->AddToViewport();
            }
        }

        GetMesh()->SetScalarParameterValueOnMaterials(ParamName_TimeToHit, GetWorld()->TimeSeconds);

        if (NewHealth <= 0.f)
        {
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }

            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");

            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            SetLifeSpan(10.f);
        }
    }
}

void ATioAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
        AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, TargetActor);
	}
}

AActor* ATioAICharacter::GetTargetActor() const
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    }
    return nullptr;
}

bool ATioAICharacter::ApplyHealth(AActor* InstigatorActor, float Amount /*= 100.f*/)
{
    return AttributeComponent->ApplyHealthChange(InstigatorActor, Amount);
}

bool ATioAICharacter::IsAlive() const
{
    return AttributeComponent->IsAlive();
}

