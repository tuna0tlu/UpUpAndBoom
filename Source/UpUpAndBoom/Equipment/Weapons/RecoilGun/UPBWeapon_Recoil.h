// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UPBWeapon_Base.h"
#include "../../../Character/UPBCharacter.h"
#include "UPBWeapon_Recoil.generated.h"

/**
 * 
 */
UCLASS()
class UPUPANDBOOM_API AUPBWeapon_Recoil : public AUPBWeapon_Base
{
	GENERATED_BODY()
	
	
	
public:
	virtual void ExecuteAbility_Implementation() override;
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float PushForce = 800.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float EnemyPushForce = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float EnemyUpwardBoost = 300.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float TraceDistance = 250.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float WallUpwardBias = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
	float DirectionBlend = .5f;
	
	
private:
	FVector CalculatePushDirection(const AUPBCharacter* OwnerCharacter) const;
	
};
