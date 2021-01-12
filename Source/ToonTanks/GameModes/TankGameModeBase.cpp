// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

ATankGameModeBase::ATankGameModeBase()
	:TargetTurrets(0)
{

}

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	this->HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor *DeadActor)
{
	if (DeadActor == this->PlayerTank) {
		this->PlayerTank->HandleDestruction();
		this->HandleGameOver(false);

		if (this->PlayerControllerRef) {
			this->PlayerControllerRef->SetPlayerEnabledState(false);
		}
	} else if (APawnTurret *DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
		DestroyedTurret->HandleDestruction();

		if (--TargetTurrets == 0) {
			this->HandleGameOver(true);
		}
	}
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
	TArray<AActor *> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), APawnTurret::StaticClass(), TurretActors);

	return TurretActors.Num();
}

void ATankGameModeBase::HandleGameStart()
{
	this->TargetTurrets = this->GetTargetTurretCount();

	this->PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	this->PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	this->GameStart();
	if (this->PlayerControllerRef) {
		this->PlayerControllerRef->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef,
											&APlayerControllerBase::SetPlayerEnabledState, true);

		this->GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	this->GameOver(PlayerWon);
}
