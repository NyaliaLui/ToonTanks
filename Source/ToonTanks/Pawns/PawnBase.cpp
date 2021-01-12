// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	this->CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = this->CapsuleComp;

	this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	this->BaseMesh->SetupAttachment(RootComponent);

	this->TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	this->TurretMesh->SetupAttachment(this->BaseMesh);

	this->ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	this->ProjectileSpawnPoint->SetupAttachment(this->TurretMesh);

	this->HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetCleaned(LookAtTarget.X, LookAtTarget.Y, this->TurretMesh->GetComponentLocation().Z);
	FVector StartLocation(this->TurretMesh->GetComponentLocation());
	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
	this->TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire(void)
{
	// Get ProjectileSpawnPoint Location and Rotation
	// then spawn Projectile class at location firing towards rotation
	if (this->ProjectileClass) {
		FVector SpawnLocation(this->ProjectileSpawnPoint->GetComponentLocation());
		FRotator SpawnRotation(this->ProjectileSpawnPoint->GetComponentRotation());

		AProjectileBase *TempProjectile = this->GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::HandleDestruction(void)
{
	// Play death particle effects, sound, and camera shake

	UGameplayStatics::SpawnEmitterAtLocation(this, this->DeathParticle, this->GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, this->DeathSound, this->GetActorLocation());
	this->GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(this->DeathShake);
}