// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBWeapon_Grapple.h"
#include "../../../Character/UPBCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


AUPBWeapon_Grapple::AUPBWeapon_Grapple()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}


void AUPBWeapon_Grapple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsGrappling) return;
	if (!CachedOwner) return;
	
	FVector CurrentLocation = CachedOwner->GetActorLocation();
	FVector ToGrapple = GrapplePoint - CurrentLocation;
	float Distance = ToGrapple.Size();
	
	if (Distance < StopDistance)
	{
		StopGrapple();
		return;
	}
	
	float PullForce = FMath::Lerp(MinPullForce, MaxPullForce,FMath::Clamp(Distance / TraceDistance, 0.f,1.f));
	FVector PullDirection = ToGrapple.GetSafeNormal();
	
	if (CachedOwner->GetCharacterMovement()->Velocity.Size() < MaxGrappleSpeed)
	{
		CachedOwner->ApplyExplosiveVelocity(PullDirection * PullForce * DeltaTime, false, false);
	}

}

void AUPBWeapon_Grapple::ExecuteAbility_Implementation()
{
	AUPBCharacter* OwnerCharacter = Cast<AUPBCharacter>(GetOwner());
	if (!OwnerCharacter) return;
	
	FHitResult HitResult;
	if (!DoLineTrace(OwnerCharacter, HitResult)) return;
	
	Multicast_StartGrapple(HitResult.ImpactPoint);
}

void AUPBWeapon_Grapple::Multicast_StartGrapple_Implementation(FVector Point)
{
	CachedOwner = Cast<AUPBCharacter>(GetOwner());
	if (!CachedOwner) return;
	
	CachedOwner->GetCharacterMovement()->GravityScale = 0.f;
	
	GrapplePoint = Point;
	bIsGrappling = true;
	SetActorTickEnabled(true);
}

void AUPBWeapon_Grapple::StopGrapple()
{
	if (!bIsGrappling) return;
	Multicast_StopGrapple();
}

void AUPBWeapon_Grapple::Multicast_StopGrapple_Implementation()
{
	bIsGrappling = false;
	SetActorTickEnabled(false);
	CachedOwner->GetCharacterMovement()->GravityScale = CachedOwner->GetDefaultGravityScale();
}

bool AUPBWeapon_Grapple::DoLineTrace(AUPBCharacter* OwnerCharacter, FHitResult& OutHit) const
{
	FVector CameraLocation = OwnerCharacter->GetCameraWorldLocation();
	FVector CameraForward = OwnerCharacter->GetBaseAimRotation().Vector();
	FVector TraceEnd = CameraLocation + CameraForward * TraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerCharacter);

	return GetWorld()->LineTraceSingleByChannel(
		OutHit,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		Params
	);
}
