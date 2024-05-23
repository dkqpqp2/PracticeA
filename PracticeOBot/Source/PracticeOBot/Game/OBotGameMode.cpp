// Fill out your copyright notice in the Description page of Project Settings.


#include "OBotGameMode.h"
#include "../Player/OBotPlayerController.h"

AOBotGameMode::AOBotGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> OBotClassRef(TEXT("/Script/PracticeOBot.OBotCharacterPlayer"));
	if (OBotClassRef.Class)
	{
		DefaultPawnClass = OBotClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AOBotPlayerController> PlayerControllerClassRef(TEXT("/Script/PracticeOBot.OBotPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
}
