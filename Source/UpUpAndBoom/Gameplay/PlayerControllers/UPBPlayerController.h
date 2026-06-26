// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UPBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UPUPANDBOOM_API AUPBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
	
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* BaseMappingContext;
	
	virtual void SetupInputComponent() override;
};
