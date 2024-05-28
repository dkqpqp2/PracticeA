// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"
#include "Interface/OBotAnimationAttackInterface.h"
#include "Projectile/PlayerProjectile.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		FVector SocketLocation = MeshComp->GetSocketLocation(TEXT("AttackSocket"));
		FRotator SocketRotation = MeshComp->GetSocketRotation(TEXT("AttackSocket"));

		UWorld* World = MeshComp->GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = MeshComp->GetOwner();
			SpawnParams.Instigator = MeshComp->GetOwner()->GetInstigator();

			World->SpawnActor<APlayerProjectile>(APlayerProjectile::StaticClass(), SocketLocation, SocketRotation, SpawnParams);
		}

	}

	
	
}

