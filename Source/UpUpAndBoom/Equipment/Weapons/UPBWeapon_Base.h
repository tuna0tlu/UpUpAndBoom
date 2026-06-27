// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UPBWeapon_Base.generated.h"

UCLASS()
class UPUPANDBOOM_API AUPBWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUPBWeapon_Base();
	
	UFUNCTION(BlueprintNativeEvent, Category="Weapon")
	void ExecuteAbility();
	virtual void ExecuteAbility_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* RootSceneComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
