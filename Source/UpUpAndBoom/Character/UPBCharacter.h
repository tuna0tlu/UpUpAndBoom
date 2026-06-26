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

protected:
	virtual void BeginPlay() override;

public: 
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Multiplayer
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;


protected:
	
	//Physics
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CustomGravityScale, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float customGravityScale = .85f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CustomAirControl, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true")) //Sonra gerekirse rep fonksiyonu bakarız
	float customAirControl = .6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CustomAirBoostMultiplier, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float customAirBoostMultiplier = 2.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CustomFallingLateralFriction, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float customFallingLateralFriction = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CustomJumpZVelocity, Category = "Movement|Physics", meta = (AllowPrivateAccess = "true"))
	float customJumpZVelocity = 450.f;
	
	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	
	//Action Functions
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void HandleJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopJump();
	
	
	
	
	//Movement Value Apply Funcs
	void ApplyGravity();
	void ApplyAirBoostMultiplier();
	void ApplyAirControl();
	void ApplyFallingLateralFriction();
	void ApplyJumpZVelocity();
	
	
	//Input Functions
	void InitializeOverlayInput();
	
	//Replication
	UFUNCTION()
	void OnRep_CustomGravityScale();
	
	UFUNCTION()
	void OnRep_CustomAirBoostMultiplier();
	
	UFUNCTION()
	void OnRep_CustomFallingLateralFriction();
	
	UFUNCTION()
	void OnRep_CustomJumpZVelocity();
	
	UFUNCTION()
	void OnRep_CustomAirControl();
};