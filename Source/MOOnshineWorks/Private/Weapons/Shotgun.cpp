// Fill out your copyright notice in the Description page of Project Settings.

#include "MOOnshineWorks.h"
#include "Shotgun.h"


AShotgun::AShotgun(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Locomotion";
	Type = EGunType::Shotgun;
	Id = 15.f;

	CharacterEquipOffset = FVector(20.f, 17.5f, -40.f);
	CharacterEquipRotation = FRotator(0.f, 80.f, 0.f);
	
	SpreadAngle = 20.f;
	ShootCooldown = 1.2f;
	PelletCount = 6.f;

	Charge = 0.f;
	IsCharging = false;
	ChargeRatePerSecond = (1.f / 3.f);
	ChargeMultiplier = 3.f;
	ChargeMovementMultiplier = 0.7f;
}

void AShotgun::Use()
{
	if (HasAmmo())
	{
		if (CanShoot())
		{
			Shoot();
			UseAmmo();
		}
	}
}

void AShotgun::Shoot()
{
	FVector SpawnLocation = RootComponent->GetSocketLocation("BulletSpawn");
	FVector Target = GetTarget();
	float OldSpread = SpreadAngle;
	if (CanCharge())
	{
		if (Charge == 1.f)
		{
			float ChargeEffectMultiplier = (Charge * ChargeMultiplier);
			SpreadAngle /= ChargeEffectMultiplier;
		}
	}
	switch (AmmoContainer->ActiveAmmoType)
	{
		default:
			for (int i = 0; i < PelletCount; i++)
			{
				AProjectile* Projectile = SpawnProjectile(SpawnLocation, Target);
			}
			break;
		case EAmmoType::Type::A:
			break;
	}
	SpreadAngle = OldSpread;
	Super::Shoot();
}

bool AShotgun::CanCharge()
{
	return true && HasAmmo() && CanShoot();
}

void AShotgun::StartCharge()
{
	Super::StartCharge();
	AMOOnshineWorksCharacter* Owner = Cast<AMOOnshineWorksCharacter>(GetOwner());
	if (Owner)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed *= ChargeMovementMultiplier;
	}
}

void AShotgun::EndCharge()
{
	Super::EndCharge();
	AMOOnshineWorksCharacter* Owner = Cast<AMOOnshineWorksCharacter>(GetOwner());
	if (Owner)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed /= ChargeMovementMultiplier;
	}
}

void AShotgun::StopCharge()
{
	AMOOnshineWorksCharacter* Owner = Cast<AMOOnshineWorksCharacter>(GetOwner());
	if (Owner && IsCharging)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed /= ChargeMovementMultiplier;
	}
	Super::StopCharge();
}