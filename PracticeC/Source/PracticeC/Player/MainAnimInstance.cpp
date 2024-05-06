// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainAnimInstance::UMainAnimInstance()
{
}

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// TryGetPawnOwner() : AnimationBlueprint를 사용하는 Pawn 객체 얻어오기
	APlayerCharacter* OwnerPawn = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (OwnerPawn)
	{
		UCharacterMovementComponent* Movement = OwnerPawn->GetCharacterMovement();
		if (Movement)
		{
			GroundSpeed = Movement->Velocity.Length();
			GroundSpeed /= Movement->MaxWalkSpeed;
		}
	}
	

}
