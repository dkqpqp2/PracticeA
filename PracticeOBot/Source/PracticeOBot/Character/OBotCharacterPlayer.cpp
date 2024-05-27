// Fill out your copyright notice in the Description page of Project Settings.


#include "OBotCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OBotControlData.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


AOBotCharacterPlayer::AOBotCharacterPlayer()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 400.f;
	CameraArm->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Jetpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack"));
	Jetpack->SetupAttachment(GetMesh(), FName("BackpackSocket"));

	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(Jetpack);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> JetpackMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StackOBot/Characters/Backpack/Mesh/SKM_Backpack.SKM_Backpack'"));
	if (JetpackMeshRef.Object)
	{
		Jetpack->SetSkeletalMesh(JetpackMeshRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/StackOBot/FX/JetpackThruster/FX_JetpackThruster.FX_JetpackThruster'"));
	if (NiagaraRef.Object)
	{
		NiagaraActivationEffect = NiagaraRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputActionChangeControlRef.Object)
	{
		ChangeControlAction = InputActionChangeControlRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsJetpackActive = false;
}

void AOBotCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	SetCharacterControl(CurrentCharacterControlType);
	
}

void AOBotCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOBotCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AOBotCharacterPlayer::StartJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOBotCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AOBotCharacterPlayer::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AOBotCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AOBotCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AOBotCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AOBotCharacterPlayer::QuaterMove);

	
}

bool AOBotCharacterPlayer::IsHovering() const
{
	return bIsHovering;
}

void AOBotCharacterPlayer::ServerStartHover_Implementation()
{
	//Server에서 StartHover 상태를 모든 플레이어에게 전파
	MulticastStartHover();
}

void AOBotCharacterPlayer::MulticastStartHover_Implementation()
{
	//모든 플레이어는 특정 캐릭터의 Hover 상태를 알고, Effect 재생
	NiagaraEffect->SetAsset(NiagaraActivationEffect);
	NiagaraEffect->Activate();
	bIsHovering = true;
	GetCharacterMovement()->AirControl = 5.f;
}

void AOBotCharacterPlayer::ServerLaunchCharacter_Implementation()
{
	MulticastLaunchCharacter();
}
void AOBotCharacterPlayer::MulticastLaunchCharacter_Implementation()
{
	LaunchCharacter(FVector(0.f, 0.f, 1.f), false, true);
}

void AOBotCharacterPlayer::ServerStopHover_Implementation()
{
	//Hover 상태가 종료된 상태를 모든 플레이어에게 전파
	MulticastStopHover();
}
void AOBotCharacterPlayer::MulticastStopHover_Implementation()
{
	NiagaraEffect->Deactivate();
	bIsHovering = false;
	GetCharacterMovement()->AirControl = 1.f;
}



void AOBotCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
	else if(CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}

}

void AOBotCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UOBotControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	if (GetController() == nullptr)
	{
		return;
	}

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

void AOBotCharacterPlayer::SetCharacterControlData(const UOBotControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraArm->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraArm->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraArm->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraArm->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraArm->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraArm->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraArm->bDoCollisionTest = CharacterControlData->bDoCollisionTest;


}

void AOBotCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void AOBotCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

void AOBotCharacterPlayer::QuaterMove(const FInputActionValue& Value)
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

void AOBotCharacterPlayer::StartJump()
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		bIsJetpackActive = false;
	}

	if (bIsJetpackActive)
	{
		ServerStartHover();
		//bIsHovering = true;
		//GetCharacterMovement()->AirControl = 5.f;
	}
	bIsJetpackActive = true;
}

void AOBotCharacterPlayer::Jump()
{
	Super::Jump();

	if (bIsHovering)
	{
		ServerLaunchCharacter();
	}
}

void AOBotCharacterPlayer::StopJumping()
{
	Super::StopJumping();

	if (bIsJetpackActive)
	{
		ServerStopHover();
	}
}

