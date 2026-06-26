#include "UPBCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

AUPBCharacter::AUPBCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(GetCapsuleComponent());
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    Camera->bUsePawnControlRotation = true;
    
    bReplicates = true;
    
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    
    CurrentGravityScale = DefaultGravityScale;
    CurrentAirControl = DefaultAirControl;
    CurrentAirBoostMultiplier = DefaultAirBoostMultiplier;
    CurrentFallingLateralFriction = DefaultFallingLateralFriction;
    CurrentJumpZVelocity = DefaultJumpZVelocity;
    
    ApplyMovementSettings();
}

void AUPBCharacter::BeginPlay()
{
    Super::BeginPlay();
}


void AUPBCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AUPBCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();
    InitializeOverlayInput();
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
    
    DOREPLIFETIME(AUPBCharacter, CurrentGravityScale);
    DOREPLIFETIME(AUPBCharacter, CurrentAirControl);
    DOREPLIFETIME(AUPBCharacter, CurrentAirBoostMultiplier);
    DOREPLIFETIME(AUPBCharacter, CurrentFallingLateralFriction);
    DOREPLIFETIME(AUPBCharacter, CurrentJumpZVelocity);
}


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

void AUPBCharacter::OnRep_CurrentGravityScale()
{
    ApplyGravity();
}
    
void AUPBCharacter::OnRep_CurrentAirBoostMultiplier()
{
    ApplyAirBoostMultiplier();
}
    
void AUPBCharacter::OnRep_CurrentFallingLateralFriction()
{
    ApplyFallingLateralFriction();
}
    
void AUPBCharacter::OnRep_CurrentJumpZVelocity()
{
    ApplyJumpZVelocity();
}

void AUPBCharacter::OnRep_CurrentAirControl()
{
    ApplyAirControl();
}

void AUPBCharacter::ApplyGravity()
{
    GetCharacterMovement()->GravityScale = CurrentGravityScale;
}
    
void AUPBCharacter::ApplyAirControl()
{
    GetCharacterMovement()->AirControl = CurrentAirControl;
}
    
void AUPBCharacter::ApplyAirBoostMultiplier()
{
    GetCharacterMovement()->AirControlBoostMultiplier = CurrentAirBoostMultiplier;
}
    
void AUPBCharacter::ApplyFallingLateralFriction()
{
    GetCharacterMovement()->FallingLateralFriction = CurrentFallingLateralFriction;
}
    
void AUPBCharacter::ApplyJumpZVelocity()
{
    GetCharacterMovement()->JumpZVelocity = CurrentJumpZVelocity;
}

void AUPBCharacter::ApplyMovementSettings()
{
    ApplyGravity();
    ApplyAirControl();
    ApplyAirBoostMultiplier();
    ApplyFallingLateralFriction();
    ApplyJumpZVelocity();
}

void AUPBCharacter::SetGravityScale(float NewGravityScale)
{
    if (HasAuthority())
    {
        CurrentGravityScale = NewGravityScale;
        ApplyGravity();
    }
}

void AUPBCharacter::SetAirControl(float NewAirControl)
{
    if (HasAuthority())
    {
        CurrentAirControl = NewAirControl;
        ApplyAirControl();
    }
}

void AUPBCharacter::SetAirBoostMultiplier(float NewMultiplier)
{
    if (HasAuthority())
    {
        CurrentAirBoostMultiplier = NewMultiplier;
        ApplyAirBoostMultiplier();  
    }

}

void AUPBCharacter::SetFallingLateralFriction(float NewFriction)
{
    if (HasAuthority())
    {
        CurrentFallingLateralFriction = NewFriction;
        ApplyFallingLateralFriction();
    }
}

void AUPBCharacter::SetJumpZVelocity(float NewJumpVelocity)
{
    if (HasAuthority())
    {
        CurrentJumpZVelocity = NewJumpVelocity;
        ApplyJumpZVelocity();
    }
}