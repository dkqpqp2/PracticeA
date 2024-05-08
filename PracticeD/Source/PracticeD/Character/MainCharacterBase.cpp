// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ComboActionData.h"

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
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
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
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AMainCharacterBase::ComboActionBegin()
{
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMainCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AMainCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AMainCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMainCharacterBase::ComboCheck, ComboEffectiveTime, false);
		AnimNotify_AttackStart();
	}

}

void AMainCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}
void AMainCharacterBase::StatrAttackTrace()
{
	SetAttackTimer();
}

void AMainCharacterBase::EndAttackTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
}

void AMainCharacterBase::SetAttackTimer()
{
	float Interval = 1.0f;

	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMainCharacterBase::AttackTrace, Interval, false);
	AttackTrace();
}

void AMainCharacterBase::AttackTrace()
{
	AttackTimer.Invalidate();

}


void AMainCharacterBase::AnimNotify_AttackStart()
{
	StatrAttackTrace(); 

	FVector LStartLocation = GetMesh()->GetSocketLocation("LeftHand_Socket");
	FVector RStartLocation = GetMesh()->GetSocketLocation("RightHand_Socket");
	FVector LEndLocation = LStartLocation + FVector(0.0, 0.0, -20.0);
	FVector REndLocation = RStartLocation + FVector(0.0, 0.0, -20.0);
	FCollisionQueryParams Params(NAME_None, false);

	FHitResult HitResult;

	bool bLHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		LStartLocation,
		LEndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeCapsule(10.0f, 20.0f),
		Params
	);

	bool bRHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		RStartLocation,
		REndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeCapsule(10.0f, 20.0f),
		Params
	);

	if (bLHit || bRHit)
	{
		// 충돌 감지
	}

	DrawDebugCapsule(
		GetWorld(),
		LStartLocation,
		20.0f,
		10.0f,
		FQuat::Identity,
		bLHit ? FColor::Green : FColor::Red,
		false,
		5.0f,
		0,
		2.0f

	);

	DrawDebugCapsule(
		GetWorld(),
		RStartLocation,
		20.0f,
		10.0f,
		FQuat::Identity,
		bRHit ? FColor::Green : FColor::Red,
		false,
		5.0f,
		0,
		2.0f
	);
}

void AMainCharacterBase::AnimNotify_AttackEnd()
{
	EndAttackTrace();
}