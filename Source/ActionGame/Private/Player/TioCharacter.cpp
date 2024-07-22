// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TioCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TioInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TioActionComponent.h"

ATioCharacter::ATioCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// ActorComponent
	InteractionComponent = CreateDefaultSubobject<UTioInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UTioActionComponent>("AtionComponent");

	ParamName_TimeToHit = "TimeToHit";
}

void ATioCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATioCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this, &ATioCharacter::OnHealthChange);
}

void ATioCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATioCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ATioCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATioCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATioCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATioCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATioCharacter::StopSprint);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATioCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ATioCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATioCharacter::Jump);
	PlayerInputComponent->BindAction("TelePort", IE_Pressed, this, &ATioCharacter::TelePort);
	PlayerInputComponent->BindAction("BlackHole", IE_Pressed, this, &ATioCharacter::BlackHole);
}

void ATioCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ATioCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// GetActorRightVector获取的是RootComponent的Y轴方向
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ATioCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ATioCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ATioCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ATioCharacter::TelePort()
{
	ActionComponent->StartActionByName(this, "TelePort");
}

void ATioCharacter::BlackHole()
{
	ActionComponent->StartActionByName(this, "BlackHole");
}

void ATioCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ATioCharacter::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(ParamName_TimeToHit, GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComponent->ApplyRageChange(InstigatorActor, RageDelta);
	}

	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(3.f);
	}
}

void ATioCharacter::HealSelf(float Amount /*= 100.f*/)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

