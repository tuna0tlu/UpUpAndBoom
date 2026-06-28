// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBWeapon_Recoil.h"
#include "GameFramework/Character.h"
#include "../../../Character/UPBCharacter.h"



void AUPBWeapon_Recoil::ExecuteAbility_Implementation()
{
	AUPBCharacter* OwnerCharacter = Cast<AUPBCharacter>(GetOwner());
	if (!OwnerCharacter) return;
	
	FVector PushDirection = CalculatePushDirection(OwnerCharacter);
	if (PushDirection.IsZero()) return;
	
	OwnerCharacter->ApplyExplosiveVelocity(PushDirection * PushForce, true, true);
}



FVector AUPBWeapon_Recoil::CalculatePushDirection(const AUPBCharacter* OwnerCharacter) const
{
	FVector CameraLocation = OwnerCharacter->GetCameraWorldLocation();
	FVector CameraForward = OwnerCharacter->GetBaseAimRotation().Vector();
	FVector TraceEnd = CameraLocation + CameraForward * TraceDistance;
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerCharacter);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		Params
	);
	
	DrawDebugLine(
		GetWorld(),
		CameraLocation,
		bHit ? HitResult.ImpactPoint : TraceEnd,
		 bHit ? FColor::Red : FColor::Green,
		 false,
2.f
	);
	
	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			10.f,
			8,
			FColor::Red,
			false,
			2.f
		);

		DrawDebugDirectionalArrow(
			GetWorld(),
			HitResult.ImpactPoint,
			HitResult.ImpactPoint + HitResult.ImpactNormal * 50.f,
			10.f,
			FColor::Yellow,
			false,
			2.f
		);
	}

	
	
	if (!bHit) return FVector::ZeroVector;
	
	if (AUPBCharacter* HitCharacter = Cast<AUPBCharacter>(HitResult.GetActor()))
	{
		FVector ToEnemy = (HitCharacter->GetActorLocation() - OwnerCharacter->GetActorLocation());
		ToEnemy.Z = 0.f;
		ToEnemy.Normalize();

		FVector EnemyPushDir = (ToEnemy + FVector(0.f, 0.f, EnemyUpwardBoost)).GetSafeNormal();
		
		HitCharacter->ApplyExplosiveVelocity(EnemyPushDir * EnemyPushForce, false, false);
		return FVector::ZeroVector;
	}
	
	FVector CameraRecoil = -CameraForward;
	FVector SurfaceNormal = HitResult.ImpactNormal;
	
	float UpDot = FVector::DotProduct(SurfaceNormal, FVector::UpVector);
	float WallFactor = 1.f - FMath::Abs(UpDot);
	FVector WallBiasedNormal = SurfaceNormal + FVector(0.f, 0.f, WallFactor * WallUpwardBias);
	WallBiasedNormal.Normalize();
	
	FVector BlendedDirection = FMath::Lerp(WallBiasedNormal, CameraRecoil, DirectionBlend);
	BlendedDirection.Normalize();
	
	return BlendedDirection;
	
}