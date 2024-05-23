// Fill out your copyright notice in the Description page of Project Settings.


#include "OBotCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OBotControlData.h"

// Sets default values
AOBotCharacterBase::AOBotCharacterBase()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCapsuleComponent()->InitCapsuleSize(35.0f, 60.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -60.0), FRotator(0.0, -90.0, 0.0));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StackOBot/Characters/Bot/Mesh/SKM_Bot.SKM_Bot'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Character/AB_OBot.AB_OBot_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UOBotControlData> ShoulderRef(TEXT("/Script/PracticeOBot.OBotControlData'/Game/Character/Control/Shoulder.Shoulder'"));
    if (ShoulderRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderRef.Object);
    }


    static ConstructorHelpers::FObjectFinder<UOBotControlData> QuaterRef(TEXT("/Script/PracticeOBot.OBotControlData'/Game/Character/Control/Quater.Quater'"));
    if (QuaterRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterRef.Object);
    }

}

void AOBotCharacterBase::SetCharacterControlData(const UOBotControlData* CharacterControlData)
{
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}


