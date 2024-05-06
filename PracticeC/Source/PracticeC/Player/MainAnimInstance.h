// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEC_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainAnimInstance();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

public:
	// virtual : 가상 함수를 선언 하는 것이고
	// override : 부모의 가상 함수를 재정의 한다는것을 나타냄
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
