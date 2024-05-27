// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"

UCLASS()
class PRACTICEOBOT_API APlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> NiagaraProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> NiagaraProjectileEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectileMovement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;


};
