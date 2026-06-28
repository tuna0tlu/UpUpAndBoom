// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/UPBWeapon_Base.h"
class AUPBCharacter;

#include "UPBWeapon_Grapple.generated.h"

/**
 * 
 */
UCLASS()
class UPUPANDBOOM_API AUPBWeapon_Grapple : public AUPBWeapon_Base
{
	GENERATED_BODY()
	
	
public:
	AUPBWeapon_Grapple();
	
	virtual void Tick(float DeltaTime) override;
	virtual void ExecuteAbility_Implementation() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartGrapple(FVector Point);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopGrapple();
	
	UFUNCTION(BlueprintCallable, Category = "Grapple")
	void StopGrapple();
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grapple Settings")
	float TraceDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grapple Settings")
	float MaxPullForce = 3000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grapple Settings")
	float MinPullForce = 500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grapple Settings")
	float StopDistance = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grapple Settings")
	float MaxGrappleSpeed = 3500.f;
	
private:
	bool bIsGrappling = false;
	FVector GrapplePoint;
	
	bool DoLineTrace(AUPBCharacter* OwnerCharacter, FHitResult& OutHit) const;
	
	UPROPERTY()
	AUPBCharacter* CachedOwner = nullptr;
};
