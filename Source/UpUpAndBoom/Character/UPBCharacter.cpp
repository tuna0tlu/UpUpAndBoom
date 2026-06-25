// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
AUPBCharacter::AUPBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Setup Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	Camera -> bUsePawnControlRotation = true;
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	
}

// Called when the game starts or when spawned
void AUPBCharacter::BeginPlay()

{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AUPBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(
	MoveAction,
	ETriggerEvent::Triggered,
	this,
	&AUPBCharacter::Move
	);
	
	EnhancedInput->BindAction(
		LookAction,
		ETriggerEvent::Triggered,
		this,
		&AUPBCharacter::Look
	);
	
	EnhancedInput->BindAction(
	JumpAction,
	ETriggerEvent::Triggered,
	this,
	&AUPBCharacter::HandleJump
	);
	
	EnhancedInput->BindAction(
	JumpAction,
	ETriggerEvent::Canceled,
	this,
	&AUPBCharacter::StopJump
	);
}

// Called every frame
void AUPBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUPBCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AUPBCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	
	AddControllerPitchInput(-LookAxis.Y);
	AddControllerYawInput(LookAxis.X);
}

void AUPBCharacter::HandleJump(const FInputActionValue& Value)
{
	Jump();
}

void AUPBCharacter::StopJump()
{
	
}


