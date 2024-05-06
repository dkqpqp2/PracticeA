// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PRACTICED_API AMainCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UMainCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UMainCharacterControlData*> CharacterControlManager;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	void ProcessComboCommand();
};
