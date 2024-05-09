// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MainCharacterControlData.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

AMainCharacterPlayer::AMainCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Crouch로 전환할 때 부드럽게 전환하는 설정
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 20.0f;
	CameraBoom->CameraRotationLagSpeed = 20.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
#if WITH_EDITORONLY_DATA
	FName LHandSocket(TEXT("LeftHand_Socket"));
	FName RHandSocket(TEXT("RightHand_Socket"));
	LArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LArrow"));
	LArrow->SetupAttachment(GetMesh(), LHandSocket);
	LArrow->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	RArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RArrow"));
	RArrow->SetupAttachment(GetMesh(), RHandSocket);
	RArrow->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
#endif

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_ChangeControlData.IA_ChangeControlData'"));
	if (nullptr != ChangeControlActionRef.Object)
	{
		ChangeControlAction = ChangeControlActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_Crouch.IA_Crouch'"));
	if (nullptr != InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PracticeContents/Player/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsCrouching = false;

	LSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LHandSphere"));
	LSphere->SetupAttachment(GetMesh(), LHandSocket);
	LSphere->SetRelativeLocationAndRotation(FVector(0.0, -5.0, 0.0), FRotator(0.0, -90.0, 0.0));
	LSphere->SetSphereRadius(10.0f);

	RSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RHandSphere"));
	RSphere->SetupAttachment(GetMesh(), RHandSocket);
	RSphere->SetRelativeLocationAndRotation(FVector(0.0, -5.0, 0.0), FRotator(0.0, -90.0, 0.0));
	RSphere->SetSphereRadius(10.0f);
	
}

void AMainCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
	LSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	RSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void AMainCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Completed, this, &AMainCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacterPlayer::OnCrouh);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMainCharacterPlayer::OnUnCrouh);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AMainCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AMainCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AMainCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacterPlayer::Attack);

}

void AMainCharacterPlayer::ActivateCollision()
{
	LSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainCharacterPlayer::DeactivateCollision()
{
	LSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if(CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AMainCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UMainCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);


	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AMainCharacterPlayer::SetCharacterControlData(const UMainCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AMainCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// IA_Move 에서 스위즐 입력 측 값 YXZ로 Modifier 추가해서 사용가능
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMainCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

void AMainCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AMainCharacterPlayer::OnCrouh(const FInputActionValue& Value)
{
	Crouch(false);
}

void AMainCharacterPlayer::OnUnCrouh(const FInputActionValue& Value)
{
	UnCrouch(false);
}

void AMainCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void AMainCharacterPlayer::OnLeftAndRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap And Damage"));

}




