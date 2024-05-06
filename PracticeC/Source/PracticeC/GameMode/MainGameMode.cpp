// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "../Player/MainPlayerController.h"

AMainGameMode::AMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerCharacterRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerCharacterRef.Class)
	{
		DefaultPawnClass = PlayerCharacterRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMainPlayerController> PlayerControllerRef(TEXT("/Game/Blueprints/BP_MainPlayerController.BP_MainPlayerController"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}
