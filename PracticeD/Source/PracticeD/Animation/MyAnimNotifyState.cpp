// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimNotifyState.h"
#include "Character/MainCharacterPlayer.h"

void UMyAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);


	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<AMainCharacterPlayer>(MeshComp->GetOwner());
		if (Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ShpereCollision Activate"));
			Character->ActivateCollision();
			DrawDebugSphere(GetWorld(), Character->GetMesh()->GetSocketLocation("LeftHand_Socket"), 20.0f, 5, FColor::Red, false, 5.f, 0, 1.f);
		}

	}

}

void UMyAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<AMainCharacterPlayer>(MeshComp->GetOwner());
		if (Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ShpereCollision Deactivate"));
			Character->DeactivateCollision();
		}

	}

}
