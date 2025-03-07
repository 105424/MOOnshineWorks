// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AmmoContainer.h"
#include "Weapons/Gun.h"
#include "PlayerGun.generated.h"

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API APlayerGun : public AGun
{
	GENERATED_BODY()
public:
	APlayerGun(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equip)
	FVector CharacterEquipOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equip)
	FRotator CharacterEquipRotation;
	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	TSubclassOf<UCameraShake> ShotFeedBack;
	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	TSubclassOf<UCameraShake> ChargeShaker;

	void GiveShotFeedBack();
	AAmmoContainer* AmmoContainer;

	virtual FVector GetTarget() override;	
	virtual void Shoot() override;
	void UseAmmo();
	bool HasAmmo();
	virtual	TSubclassOf<class AProjectile> GetProjectileClass() override;
	void SetActiveGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	TArray<TEnumAsByte<EAmmoType::Type>> AmmoTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	TArray<int32> Multipliers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	TArray<TSubclassOf<class AProjectile>> ProjectileClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ActiveIndex;
	void SetActiveIndex(EAmmoType::Type);
	EAmmoType::Type FindActiveAmmoType();
	int32 FindActiveMultiplier();
	TSubclassOf<class AProjectile> FindActiveProjectileClass();

	UFUNCTION(BlueprintCallable, Category = Ammo)
	int32 GetRemainingShotCount();

	UFUNCTION(BlueprintNativeEvent, Category = Ammo)
	void SetVisibility(bool Visible);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	UTexture2D* HUDTexture;

	float LastChargeShakerScale;
	virtual void StartCharge() override;
	virtual void EndCharge() override;
	virtual void StopCharge() override;
	virtual void Tick(float DeltaSeconds);
};