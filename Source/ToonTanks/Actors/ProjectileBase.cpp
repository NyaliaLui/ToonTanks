// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
	:ProjectileMovement(nullptr),
	ProjectileMesh(nullptr),
	MovementSpeed(1300),
	Damage(50)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = this->ProjectileMesh;

	this->ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	this->ProjectileMovement->InitialSpeed = this->MovementSpeed;
	this->ProjectileMovement->MaxSpeed = this->MovementSpeed;
	InitialLifeSpan = 3.0f;

	this->ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	this->ParticleTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	this->ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, this->LaunchSound, this->GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	AActor *MyOwner = this->GetOwner();

	if (!MyOwner) {
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		UGameplayStatics::ApplyDamage(OtherActor, this->Damage, MyOwner->GetInstigatorController(), this, this->DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(this, this->HitParticle, this->GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, this->HitSound, this->GetActorLocation());
		this->GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(this->HitShake);
		Destroy();
	}
}