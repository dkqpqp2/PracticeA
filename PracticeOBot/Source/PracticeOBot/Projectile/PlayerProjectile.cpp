// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APlayerProjectile::APlayerProjectile()
{
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	NiagaraProjectile = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Fly/Others/energy_08_01.energy_08_01'"));
	if (EffectRef.Object)
	{
		NiagaraProjectileEffect = EffectRef.Object;
	}

	RootComponent = SphereTrigger;
	NiagaraProjectile->SetupAttachment(SphereTrigger);
	NiagaraProjectile->SetAsset(NiagaraProjectileEffect);
	NiagaraProjectile->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	
}



