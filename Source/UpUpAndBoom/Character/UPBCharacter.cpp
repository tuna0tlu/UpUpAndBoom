#include "UPBCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "../Equipment/Weapons/GrappleGun/UPBWeapon_Grapple.h"



// Constructor

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


// Engine Lifecycle

void AUPBCharacter::BeginPlay()
{
    Super::BeginPlay();

   if (HasAuthority())
   {
       FActorSpawnParameters SpawnParams;
       SpawnParams.Owner = this;
       SpawnParams.Instigator = GetInstigator();

       if (RecoilGunClass)
       {
           RecoilGun = GetWorld()->SpawnActor<AUPBWeapon_Base>(RecoilGunClass, GetActorLocation(), GetActorRotation(), SpawnParams);
       }

       if (GrappleGunClass)
       {
           GrappleGun = GetWorld()->SpawnActor<AUPBWeapon_Base>(GrappleGunClass, GetActorLocation(), GetActorRotation(), SpawnParams);
       }

       // Başlangıçta Recoil aktif
       ActiveWeapon = RecoilGun;
   }
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


// Input

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

void AUPBCharacter::HandleSelectRecoil(const FInputActionValue& Value)
{
    Server_SelectRecoil();
}

void AUPBCharacter::Server_SelectRecoil_Implementation()
{
    ActiveWeapon = RecoilGun;
}

void AUPBCharacter::HandleSelectGrapple(const FInputActionValue& Value)
{
    Server_SelectGrapple();
}

void AUPBCharacter::Server_SelectGrapple_Implementation()
{
    ActiveWeapon = GrappleGun;
}

void AUPBCharacter::HandleFire(const FInputActionValue& Value)
{
    Server_HandleFire();
}

void AUPBCharacter::Server_HandleFire_Implementation()
{
    if (ActiveWeapon)
    {
        ActiveWeapon->ExecuteAbility();
    }
}

void AUPBCharacter::HandleStopFire()
{
    Server_HandleStopFire();
}

void AUPBCharacter::Server_HandleStopFire_Implementation()
{
    if (AUPBWeapon_Grapple* Grapple = Cast<AUPBWeapon_Grapple>(ActiveWeapon))
    {
        Grapple->StopGrapple();
    }
}

void AUPBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AUPBCharacter::Move);

        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &AUPBCharacter::Look);

        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &AUPBCharacter::HandleJump);

        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &AUPBCharacter::StopJump);

        EnhancedInputComponent->BindAction(
            SelectRecoilAction,
            ETriggerEvent::Started,
            this,
            &AUPBCharacter::HandleSelectRecoil);

        EnhancedInputComponent->BindAction(
            SelectGrappleAction,
            ETriggerEvent::Started,
            this,
            &AUPBCharacter::HandleSelectGrapple);

        EnhancedInputComponent->BindAction(
            FireAction,
            ETriggerEvent::Started,
            this,
            &AUPBCharacter::HandleFire);

        EnhancedInputComponent->BindAction(
            FireAction,
            ETriggerEvent::Completed,
            this,
            &AUPBCharacter::HandleStopFire);
    }
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




// Replication

void AUPBCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AUPBCharacter, CurrentGravityScale);
    DOREPLIFETIME(AUPBCharacter, CurrentAirControl);
    DOREPLIFETIME(AUPBCharacter, CurrentAirBoostMultiplier);
    DOREPLIFETIME(AUPBCharacter, CurrentFallingLateralFriction);
    DOREPLIFETIME(AUPBCharacter, CurrentJumpZVelocity);
    DOREPLIFETIME(AUPBCharacter, RecoilGun);
    DOREPLIFETIME(AUPBCharacter, GrappleGun);
    DOREPLIFETIME(AUPBCharacter, ActiveWeapon);
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




//Movement Functions
void AUPBCharacter::ApplyExplosiveVelocity(FVector ImpulseForce, bool bOverrideXY, bool bOverrideZ)
{
    if (HasAuthority() || IsLocallyControlled())
    {
        LaunchCharacter(ImpulseForce, bOverrideXY, bOverrideZ);
    }
}

FVector AUPBCharacter::GetCameraWorldLocation() const
{
    return Camera->GetComponentLocation();
}

// Movement Settings - Apply

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

    GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
}



// Movement Settings - Setters

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
