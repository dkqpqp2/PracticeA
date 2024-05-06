// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	CameraArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(CameraArm);

	CameraArm->SetRelativeLocation(FVector(0.0, 0.0, 54.0));
	// Y, Z, X 회전
	CameraArm->SetRelativeRotation(FRotator(-15.0, 0.0, 0.0));


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

