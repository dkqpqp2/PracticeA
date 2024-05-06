// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterControlData.h"
#include "Animation/AnimMontage.h"

// Sets default values
AMainCharacterBase::AMainCharacterBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PracticeContents/Player/Archer/Erika_Archer.Erika_Archer'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/PracticeContents/Player/Animation/AB_Archer.AB_Archer_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMainCharacterControlData> ShoulderDataRef(TEXT("/Script/PracticeD.MainCharacterControlData'/Game/PracticeContents/CharacterControl/Shoulder.Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMainCharacterControlData> QuaterDataRef(TEXT("/Script/PracticeD.MainCharacterControlData'/Game/PracticeContents/CharacterControl/Quater.Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}


}

void AMainCharacterBase::SetCharacterControlData(const UMainCharacterControlData* CharacterControlData)
{
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AMainCharacterBase::ProcessComboCommand()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
}
