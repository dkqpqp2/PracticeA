// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APlayerProjectile::APlayerProjectile()
{
 	
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	NiagaraProjectile = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	RootComponent = SphereTrigger;
	NiagaraProjectile->SetupAttachment(SphereTrigger);

	static ConstructorHelpers::FObjectFinder<UNiagaraComponent> NiagaraEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Fly/Others/Trail_09_02.Trail_09_02'"));
	if (NiagaraEffectRef.Object)
	{
		NiagaraProjectile = NiagaraEffectRef.Object;
	}

	ProjectileMovement->InitialSpeed = 100.f;
	ProjectileMovement->MaxSpeed = 100.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}


