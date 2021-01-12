// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APawnTurret::APawnTurret()
    :FireRate(2.0f),
    FireRange(500.0f)
{
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	this->GetWorld()->GetTimerManager().SetTimer(this->FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, this->FireRate, true);

    this->PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!this->PlayerPawn || this->ReturnDistanceToPlayer() > this->FireRange) {
        return;
    }

    this->RotateTurret(this->PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition(void)
{
    // If player is null or is Dead Then bail
    if (!this->PlayerPawn && !this->PlayerPawn->GetIsPlayerAlive()) {
        return;
    }

    // If player is in range, then fire!
    if (this->ReturnDistanceToPlayer() <= this->FireRange) {
        this->Fire();
    }
}

float APawnTurret::ReturnDistanceToPlayer(void)
{
    // If player is null or is Dead Then bail
    if (!this->PlayerPawn) {
        return 0.0f;
    }

    return FVector::Dist(this->PlayerPawn->GetActorLocation(), this->GetActorLocation());
}

void APawnTurret::HandleDestruction(void)
{
	Super::HandleDestruction();
    this->Destroy();
}