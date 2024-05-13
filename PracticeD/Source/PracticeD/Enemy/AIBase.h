// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIBase.generated.h"

UCLASS()
class PRACTICED_API AAIBase : public APawn
{
	GENERATED_BODY()

public:
	AAIBase();

	UCapsuleComponent* Root;
	USkeletalMeshComponent* Mesh;
	UFloatingPawnMovement* Movement;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
