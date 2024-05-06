// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/PracticeContents/Blueprint/BP_MainCharacterPlayer.BP_MainCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PracticeD.MainPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	//PlayerControllerClass = AMainPlayerController::StaticClass();
}
