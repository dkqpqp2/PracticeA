// Fill out your copyright notice in the Description page of Project Settings.


#include "OBotGameMode.h"
#include "Player/OBotPlayerController.h"

AOBotGameMode::AOBotGameMode()
{
	// DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> OBotClassRef(TEXT("/Game/StackOBot/Blueprints/Character/BP_Bot.BP_Bot_C"));
	if (OBotClassRef.Class)
	{
		DefaultPawnClass = OBotClassRef.Class;
	}
	PlayerControllerClass = AOBotPlayerController::StaticClass();
}
