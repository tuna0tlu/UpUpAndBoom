#include "UPBCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Enhanced Input başlıkları
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

AUPBCharacter::AUPBCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Setup Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(GetCapsuleComponent());
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    Camera->bUsePawnControlRotation = true;
    
    bReplicates = true;
    
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    
    ApplyGravity();
    ApplyAirControl();
    ApplyAirBoostMultiplier();
    ApplyFallingLateralFriction();
    ApplyJumpZVelocity();
    
}

void AUPBCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeOverlayInput();
}


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
    StopJumping(); 
}

void AUPBCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AUPBCharacter, customGravityScale);
    DOREPLIFETIME(AUPBCharacter, customAirControl);
    DOREPLIFETIME(AUPBCharacter, customAirBoostMultiplier);
    DOREPLIFETIME(AUPBCharacter, customFallingLateralFriction);
    DOREPLIFETIME(AUPBCharacter, customJumpZVelocity);
}


//Input
void AUPBCharacter::InitializeOverlayInput()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void AUPBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUPBCharacter::Move);
        
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUPBCharacter::Look);
        
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUPBCharacter::HandleJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUPBCharacter::StopJump);
    }
}

//Replication
void AUPBCharacter::OnRep_CustomGravityScale()
{
    ApplyGravity();
}
    
void AUPBCharacter::OnRep_CustomAirBoostMultiplier()
{
    ApplyAirBoostMultiplier();
}
    
void AUPBCharacter::OnRep_CustomFallingLateralFriction()
{
    ApplyFallingLateralFriction();
}
    
void AUPBCharacter::OnRep_CustomJumpZVelocity()
{
    ApplyJumpZVelocity();
}

void AUPBCharacter::OnRep_CustomAirControl()
{
    ApplyAirControl();
}

//Movement Setting
void AUPBCharacter::ApplyGravity()
{
    GetCharacterMovement()->GravityScale = customGravityScale;
}
    
void AUPBCharacter::ApplyAirControl()
{
    GetCharacterMovement()->AirControl = customAirControl;
}
    
void AUPBCharacter::ApplyAirBoostMultiplier()
{
    GetCharacterMovement()->AirControlBoostMultiplier = customAirBoostMultiplier;
}
    
void AUPBCharacter::ApplyFallingLateralFriction()
{
    GetCharacterMovement()-> FallingLateralFriction = customFallingLateralFriction;
}
    
void AUPBCharacter::ApplyJumpZVelocity()
{
    GetCharacterMovement()-> JumpZVelocity = customJumpZVelocity;
}