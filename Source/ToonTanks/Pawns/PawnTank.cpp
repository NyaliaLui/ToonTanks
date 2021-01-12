// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"

// Sets default values
APawnTank::APawnTank()
	:MoveSpeed(100.0f),
	RotateSpeed(100.0f),
	PlayerControllerRef(nullptr)
{
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    this->SpringArm->SetupAttachment(RootComponent);

	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	this->Camera->SetupAttachment(this->SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
	this->PlayerControllerRef = Cast<APlayerController>(this->GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	this->Rotate();
	this->Move();

	if (this->PlayerControllerRef) {
		FHitResult TraceHitResult;
		this->PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation(TraceHitResult.ImpactPoint);

		this->RotateTurret(HitLocation);
	}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{
	this->MoveDirection = FVector(Value * this->MoveSpeed * this->GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * this->RotateSpeed * this->GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	this->RotationDirection = FQuat(Rotation);
}

void APawnTank::Move(void)
{
	this->AddActorLocalOffset(this->MoveDirection, true);
}

void APawnTank::Rotate(void)
{
	this->AddActorLocalRotation(this->RotationDirection, true);
}

void APawnTank::HandleDestruction(void)
{
	Super::HandleDestruction();

	// Hide Player
	this->bIsPlayerAlive = false;

	this->SetActorHiddenInGame(true);
	this->SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive()
{
	return this->bIsPlayerAlive;
}