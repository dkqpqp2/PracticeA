// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInputDataConfig.h"

UMainInputDataConfig::UMainInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Input/IMC_Main.IMC_Main'") };
		if (InputMappingContextRef.Succeeded())
		{
			DefaultContext = InputMappingContextRef.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> InputMoveActionRef{ TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_Move.IA_Move'") };
		if (InputMoveActionRef.Succeeded())
		{
			Move = InputMoveActionRef.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> InputLookActionRef{ TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_Look.IA_Look'") };
		if (InputLookActionRef.Succeeded())
		{
			Look = InputLookActionRef.Object;
		}
	}
}
