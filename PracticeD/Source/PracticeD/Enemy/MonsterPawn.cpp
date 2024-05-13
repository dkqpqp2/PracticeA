// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterPawn.h"
#include "AI/DefaultAIController.h"

AMonsterPawn::AMonsterPawn()
{
	AIControllerClass = ADefaultAIController::StaticClass();
}
