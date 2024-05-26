// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OBotCharacterBase.generated.h"


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PRACTICEOBOT_API AOBotCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOBotCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UOBotControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UOBotControlData*> CharacterControlManager;
};
