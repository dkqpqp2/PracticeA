// Fill out your copyright notice in the Description page of Project Settings.


#include "OBotAnimInstance.h"
#include "Character/OBotCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UOBotAnimInstance::UOBotAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UOBotAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// AnimInstance를 사용하는 Actor를 가져올수잇음
	Owner = Cast<AOBotCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UOBotAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsHovering = Owner->IsHovering();
	}
}
