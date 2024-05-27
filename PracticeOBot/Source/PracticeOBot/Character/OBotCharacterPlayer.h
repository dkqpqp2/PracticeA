// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OBotCharacterBase.h"
#include "OBotCharacterPlayer.generated.h"

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class PRACTICEOBOT_API AOBotCharacterPlayer : public AOBotCharacterBase
{
	GENERATED_BODY()

public:
	AOBotCharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsHovering() const;
	UFUNCTION(Server, Unreliable)
	void ServerStartHover();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStartHover();

	UFUNCTION(Server, Unreliable)
	void ServerLaunchCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLaunchCharacter();

	UFUNCTION(Server, Unreliable)
	void ServerStopHover();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStopHover();
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const UOBotControlData* CharacterControldata) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Jetpack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> NiagaraActivationEffect;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void StartJump();
	virtual void Jump() override;
	virtual void StopJumping() override;


private:
	bool bIsJetpackActive;
	bool bIsHovering;
	FVector HoveringOriginLocation = FVector::ZeroVector;

	FTimerHandle FlightTimerHandle;
};
