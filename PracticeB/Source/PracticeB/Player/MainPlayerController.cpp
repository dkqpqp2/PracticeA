// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Data/Input/MainInputDataConfig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	const UMainInputDataConfig* MainInputConfig = GetDefault<UMainInputDataConfig>();
	Subsystem->AddMappingContext(MainInputConfig->DefaultContext, 0);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	const UMainInputDataConfig* MainInputDataConfig = GetDefault<UMainInputDataConfig>();
	EnhancedInputComponent->BindAction(MainInputDataConfig->Move, ETriggerEvent::Triggered, this, &AMainPlayerController::OnMove);
	EnhancedInputComponent->BindAction(MainInputDataConfig->Look, ETriggerEvent::Triggered, this, &AMainPlayerController::OnLook);
}

void AMainPlayerController::OnMove(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();

	const FRotator Rotation = K2_GetActorRotation();
	const FRotator YawRotation = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = YawRotation.Vector();
	const FVector RightVector = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);


	const FVector2D ActionValue = Value.Get<FVector2D>();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);

	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

	MoveDir = ActionValue.X * 90.f;

	if (ActionValue.Y > 0.f)
	{

		if (ActionValue.X < 0.f)
			MoveDir = -45.f;
		else if (ActionValue.X > 0.f)
			MoveDir = 45.f;
	}

	else if (ActionValue.Y < 0.f)
	{
		if (ActionValue.X < 0.f)
			MoveDir = -135.f;
		else if (ActionValue.X > 0.f)
			MoveDir = 135.f;
		else
			MoveDir = 180.f;
	}
}

void AMainPlayerController::OnLook(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
}
