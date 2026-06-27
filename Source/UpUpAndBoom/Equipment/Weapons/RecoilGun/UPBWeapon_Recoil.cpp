// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBWeapon_Recoil.h"
#include "GameFramework/Character.h"
#include "../../../Character/UPBCharacter.h"



void AUPBWeapon_Recoil::ExecuteAbility_Implementation()
{
	AUPBCharacter* OwnerCharacter = Cast<AUPBCharacter>(GetOwner());
	if (!OwnerCharacter) return;
	
	FVector CameraForward = OwnerCharacter->GetBaseAimRotation().Vector();
	FVector RecoilDirection = -CameraForward;
	
	FVector LaunchVelocity = RecoilDirection*PushForce;
	
	OwnerCharacter->ApplyExplosiveVelocity(LaunchVelocity);
}
