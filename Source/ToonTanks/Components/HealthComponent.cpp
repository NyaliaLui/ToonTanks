// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	:DefaultHealth(100.0f),
	Health(100.0f),
	GameModeRef(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Health = this->DefaultHealth;
	this->GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
	this->GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}


void UHealthComponent::TakeDamage(AActor *DamageActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser)
{
	if (Damage == 0 || Health <= 0) {
		return;
	}

	this->Health = FMath::Clamp(this->Health - Damage, 0.0f, this->DefaultHealth);

	if (this->Health <= 0) {
		if (this->GameModeRef) {
			this->GameModeRef->ActorDied(GetOwner());
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Health Component has no reference to the GameMode"));
		}
	}
}