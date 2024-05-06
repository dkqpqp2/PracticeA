// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h" 
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MainInputDataConfig.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEB_API UMainInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UMainInputDataConfig();

	UInputMappingContext* DefaultContext = nullptr;

	UInputAction* Move = nullptr;
	UInputAction* Look = nullptr;
	
};
