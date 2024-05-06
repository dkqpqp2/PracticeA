// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class PRACTICEB_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	float	MoveDir;

public:
	float GetMoveDir()
	{
		return MoveDir;
	}

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	
};
