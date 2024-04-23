// Fill out your copyright notice in the Description page of Project Settings.


#include "TioBTTask_RangedAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UTioBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KSTargetActor.SelectedKeyName));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector HandLocation = MyPawn->GetMesh()->GetSocketLocation(HandSocket);
		FVector Direction = TargetActor->GetActorLocation() - HandLocation;
		FRotator SpawnRotation = Direction.Rotation();

		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);
		FActorSpawnParameters SpwanParams;
		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwanParams.Instigator = MyPawn;

		AActor* SpwanActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpwanParams);
		return SpwanActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
