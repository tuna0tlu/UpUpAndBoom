// Fill out your copyright notice in the Description page of Project Settings.


#include "UPBWeapon_Base.h"

// Sets default values
AUPBWeapon_Base::AUPBWeapon_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AUPBWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUPBWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUPBWeapon_Base::ExecuteAbility_Implementation()
{
	//Overrides from child weapon classes
}