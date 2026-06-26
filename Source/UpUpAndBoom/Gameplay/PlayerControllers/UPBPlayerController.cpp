// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBPlayerController.h"

void AUPBPlayerController::SetupInputComponent()
{
	
	Super::SetupInputComponent();
	
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (BaseMappingContext)
			{
				Subsystem->AddMappingContext(
				BaseMappingContext,
				0
				);
			}
		}
	}
	
}
