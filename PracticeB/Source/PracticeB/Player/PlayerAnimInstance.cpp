// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* OwnerPawn = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(OwnerPawn))
	{
		UCharacterMovementComponent* Movement = OwnerPawn->GetCharacterMovement();
		if (IsValid(Movement))
		{
			MoveSpeed = Movement->Velocity.Length();
			MoveSpeed /= Movement->MaxWalkSpeed;
		}
	}
	

}
