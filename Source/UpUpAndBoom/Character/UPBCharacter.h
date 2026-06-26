#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "UPBCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class UPUPANDBOOM_API AUPBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUPBCharacter();

	void SetGravityScale(float NewGravityScale);
	void SetAirControl(float NewAirControl);
	void SetAirBoostMultiplier(float NewMultiplier);
	void SetFallingLateralFriction(float NewFriction);
	void SetJumpZVelocity(float NewJumpVelocity);

protected:
	virtual void BeginPlay() override;
	
	virtual void PawnClientRestart() override;

public: 
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;


protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float DefaultGravityScale = .85f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float DefaultAirControl = .6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float DefaultAirBoostMultiplier = 2.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float DefaultFallingLateralFriction = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float DefaultJumpZVelocity = 450.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGravityScale)
	float CurrentGravityScale;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAirControl)
	float CurrentAirControl;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAirBoostMultiplier)
	float CurrentAirBoostMultiplier;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentFallingLateralFriction)
	float CurrentFallingLateralFriction;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentJumpZVelocity)
	float CurrentJumpZVelocity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void HandleJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopJump();
	
	
	
	
	void ApplyGravity();
	void ApplyAirBoostMultiplier();
	void ApplyAirControl();
	void ApplyFallingLateralFriction();
	void ApplyJumpZVelocity();
	void ApplyMovementSettings();
	
	
	void InitializeOverlayInput();
	
	UFUNCTION()
	void OnRep_CurrentGravityScale();
	
	UFUNCTION()
	void OnRep_CurrentAirBoostMultiplier();
	
	UFUNCTION()
	void OnRep_CurrentFallingLateralFriction();
	
	UFUNCTION()
	void OnRep_CurrentJumpZVelocity();
	
	UFUNCTION()
	void OnRep_CurrentAirControl();
};