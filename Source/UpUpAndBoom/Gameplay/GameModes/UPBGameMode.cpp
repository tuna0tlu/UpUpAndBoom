// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBGameMode.h"
#include "../Source/UpUpAndBoom/Character/UPBCharacter.h"
#include "../PlayerControllers/UPBPlayerController.h"


AUPBGameMode::AUPBGameMode()
{
	DefaultPawnClass = AUPBCharacter::StaticClass();
	PlayerControllerClass = AUPBPlayerController::StaticClass();
}