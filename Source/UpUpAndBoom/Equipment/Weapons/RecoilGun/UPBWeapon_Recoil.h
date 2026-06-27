// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UPBWeapon_Base.h"
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
	float PushForce = 1.f;
};
