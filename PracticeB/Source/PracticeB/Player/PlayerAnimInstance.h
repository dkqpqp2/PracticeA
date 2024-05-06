// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEB_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float MoveDir;
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds )override;
};
