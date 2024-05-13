// Fill out your copyright notice in the Description page of Project Settings.


#include "RightHandAnimNotifyState.h"
#include "Character/MainCharacterPlayer.h"

void URightHandAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<AMainCharacterPlayer>(MeshComp->GetOwner());
		if (Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("RightShpereCollision Activate"));
			Character->RightActivateCollision();
		}
	}

}

void URightHandAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<AMainCharacterPlayer>(MeshComp->GetOwner());
		if (Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Right ShpereCollision Deactivate"));
			Character->RightDeactivateCollision();
		}
	}
}
