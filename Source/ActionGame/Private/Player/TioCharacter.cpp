// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TioCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TioInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"

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

	AttackAnimDelay = 0.16f;
	TraceDistance = 1000.f;
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

void ATioCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATioCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATioCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

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

void ATioCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_AttackDelay, this, &ATioCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);	
}

void ATioCharacter::PrimaryAttack_TimeElapsed()
{
	// 从相机位置进行LineTrace
	FHitResult Hit; 
	FVector TraceStart = CameraComponent->GetComponentLocation() + GetViewRotation().Vector() * 400.f;
	FVector TraceEnd = TraceStart + GetControlRotation().Vector() * TraceDistance; //距离过小还是会偏移，因为没有hit结果

	FCollisionObjectQueryParams HitObjectQueryParams;
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, HitObjectQueryParams, Shape, QueryParams);

	if (bBlockingHit)
	{
		TraceEnd = Hit.ImpactPoint;
	}
	/*DrawDebugSphere(GetWorld(), TraceStart, 30.f, 16, FColor::Yellow, false, 10.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), TraceEnd, 30.f, 16, FColor::Blue, false, 10.f, 0, 5.f);*/

	// 生成
	FVector HandLocation = GetMesh()->GetSocketLocation(SocketName);
	// 生成起点和LineTrace起点不同
	FRotator ProRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	FTransform SpawnTM = FTransform(ProRotation, HandLocation);

	//DrawDebugSphere(GetWorld(), HandLocation, 30.f, 16, FColor::Red, false, 10.f, 0, 5.f);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 10.f, 0, 5.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ATioCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ATioCharacter::TelePort()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &ATioCharacter::TelePort_TimeElapsed, AttackAnimDelay);
}

void ATioCharacter::TelePort_TimeElapsed()
{
	FHitResult Hit;
	FVector TraceStart = CameraComponent->GetComponentLocation() + GetViewRotation().Vector() * 400.f;
	FVector TraceEnd = TraceStart + GetControlRotation().Vector() * TraceDistance; //距离过小还是会偏移，因为没有hit结果

	FCollisionObjectQueryParams HitObjectQueryParams;
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, HitObjectQueryParams, Shape, QueryParams);

	if (bBlockingHit)
	{
		TraceEnd = Hit.ImpactPoint;
	}
	/*DrawDebugSphere(GetWorld(), TraceStart, 30.f, 16, FColor::Yellow, false, 10.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), TraceEnd, 30.f, 16, FColor::Blue, false, 10.f, 0, 5.f);*/

	// 生成
	FVector HandLocation = GetMesh()->GetSocketLocation(SocketName);
	// 生成起点和LineTrace起点不同
	FRotator ProRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	FTransform SpawnTM = FTransform(ProRotation, HandLocation);

	//DrawDebugSphere(GetWorld(), HandLocation, 30.f, 16, FColor::Red, false, 10.f, 0, 5.f);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 10.f, 0, 5.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(TelePortClass, SpawnTM, SpawnParams);
}

void ATioCharacter::BlackHole()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleDelay, this, &ATioCharacter::BlackHole_TimeElapsed, AttackAnimDelay);
}

void ATioCharacter::BlackHole_TimeElapsed()
{
	FHitResult Hit;
	FVector TraceStart = CameraComponent->GetComponentLocation() + GetViewRotation().Vector() * 400.f;
	FVector TraceEnd = TraceStart + GetControlRotation().Vector() * TraceDistance; //距离过小还是会偏移，因为没有hit结果

	FCollisionObjectQueryParams HitObjectQueryParams;
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	HitObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, HitObjectQueryParams, Shape, QueryParams);

	if (bBlockingHit)
	{
		TraceEnd = Hit.ImpactPoint;
	}
	/*DrawDebugSphere(GetWorld(), TraceStart, 30.f, 16, FColor::Yellow, false, 10.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), TraceEnd, 30.f, 16, FColor::Blue, false, 10.f, 0, 5.f);*/

	// 生成
	FVector HandLocation = GetMesh()->GetSocketLocation(SocketName);
	// 生成起点和LineTrace起点不同
	FRotator ProRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	FTransform SpawnTM = FTransform(ProRotation, HandLocation);

	//DrawDebugSphere(GetWorld(), HandLocation, 30.f, 16, FColor::Red, false, 10.f, 0, 5.f);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 10.f, 0, 5.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTM, SpawnParams);
}

void ATioCharacter::OnHealthChange(AActor* InstigatorActor, UTioAttributeComponent* AttributeComp, float NewHealth, float Delta)
{
	
}

