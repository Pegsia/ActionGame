// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TioAnimInstance.generated.h"

class UTioActionComponent;

UCLASS()
class ACTIONGAME_API UTioAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UTioActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsStunned;

	//~ Begin UAnimInstance Interface
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ End UAnimInstance Interface
};
