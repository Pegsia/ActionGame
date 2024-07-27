// Fill out your copyright notice in the Description page of Project Settings.


#include "TioActionEffect.h"
#include "Components/TioActionComponent.h"
#include "GameFramework/GameStateBase.h"

UTioActionEffect::UTioActionEffect()
{
	bAutoStart = true;
}

void UTioActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if (Duration > 0.0f)
	{
		FTimerDelegate D_Delegate;
		D_Delegate.BindUFunction(this, "StopAction", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, D_Delegate, Duration, false);
	}
	if (Period > 0.0f)
	{
		FTimerDelegate P_Delegate;
		P_Delegate.BindUFunction(this, "ExecutePeriodicEffect", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, P_Delegate, Period, true);
	}
}

void UTioActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);// 剩余时间来不及执行一次period，手动触发
	}

	Super::StopAction_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);

	UTioActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp)
	{
		ActionComp->RemoveAction(InstigatorActor, this);
	}
}

void UTioActionEffect::ExecutePeriodicEffect_Implementation(AActor* InstigatorActor)
{
	AActor* OwningActor = GetOwningActor();
	if (OwningActor && OwningActor->IsPendingKill()) 
	{
		return; // 被击杀后停止活动
	}
}

float UTioActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = StartTime + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	return Duration;
}
