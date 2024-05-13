// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICED_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADefaultAIController();

protected:
	UBehaviorTree* BehaviorTree;
	UBlackboardData* Blackboard;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
