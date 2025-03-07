// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/PlayerGun.h"
#include "Bazooka.generated.h"

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API ABazooka : public APlayerGun
{
	GENERATED_BODY()
public:
	ABazooka(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Charge)
	float ChargeMovementMultiplier;

	virtual void Use() override;
	virtual void Shoot() override;
	virtual bool CanCharge() override;
	virtual void StartCharge() override;
	virtual void EndCharge() override;
	virtual void StopCharge() override;
};
