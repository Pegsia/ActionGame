// Fill out your copyright notice in the Description page of Project Settings.


#include "TioAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UTioAction_ProjectileAttack::UTioAction_ProjectileAttack()
{
	AttackAnimDelay = 0.16f;
	SocketName = "Muzzle_01"; 
	TraceDistance = 5000.f;
	SweepRadius = 20.f;
}

void UTioAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), SocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if (InstigatorActor->HasAuthority())
		{
			FTimerHandle TimeHandle_PrimaryAttack;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "PrimaryAttack_TimeElapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimeHandle_PrimaryAttack, Delegate, AttackAnimDelay, false);
		}
	}
}

void UTioAction_ProjectileAttack::PrimaryAttack_TimeElapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FHitResult Hit;
		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		FVector TraceEnd = TraceStart + TraceDirection * TraceDistance; //距离过小还是会偏移，因为没有hit结果

		FVector AdjustedTraceEnd = TraceEnd;

		FCollisionObjectQueryParams HitObjectQueryParams;
		HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		HitObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, HitObjectQueryParams, Shape, QueryParams);

		if (bBlockingHit)
		{
			AdjustedTraceEnd = Hit.ImpactPoint;
		}
		/*DrawDebugSphere(GetWorld(), TraceStart, 30.f, 16, FColor::Yellow, false, 10.f, 0, 5.f);
		DrawDebugSphere(GetWorld(), TraceEnd, 30.f, 16, FColor::Blue, false, 10.f, 0, 5.f);*/

		// 生成
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(SocketName);
		// 生成起点和LineTrace起点不同
		FRotator ProRotation = FRotationMatrix::MakeFromX(AdjustedTraceEnd - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(ProRotation, HandLocation);

		// TODO: SetRotation
		//SetActorRotation(ProRotation);

		//DrawDebugSphere(GetWorld(), HandLocation, 30.f, 16, FColor::Red, false, 10.f, 0, 5.f);
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 10.f, 0, 5.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}