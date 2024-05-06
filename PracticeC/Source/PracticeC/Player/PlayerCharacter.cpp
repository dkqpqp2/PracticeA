// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	CameraArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(CameraArm);

	CameraArm->SetRelativeLocationAndRotation(FVector(0.0, 0.0, 54.0), FRotator(-15.0, 0.0, 0.0));
	CameraArm->TargetArmLength = 500.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Player/Archer/Erika_Archer.Erika_Archer'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Blueprints/Player/AB_Player.AB_Player_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Input/IMC_Main.IMC_Main'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_Move.IA_Move'"));
	if (nullptr != InputMoveActionRef.Object)
	{
		MoveAction = InputMoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputLookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_Look.IA_Look'"));
	if (nullptr != InputLookActionRef.Object)
	{
		LookAction = InputLookActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputDodgeActionRef(TEXT(""));
	if (nullptr != InputDodgeActionRef.Object)
	{
		DodgeAction = InputDodgeActionRef.Object;
	}



}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &APlayerCharacter::Dodge);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.Y);
	AddControllerPitchInput(LookAxisVector.X);
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
}
