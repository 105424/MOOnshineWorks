// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AI_BasicEnemy.h"
#include "AI_BasicController.h"
#include "Gun.h"
#include "AIBookGun.h"
#include "AI_RangeEnemy.h"
#include "AI_BookEnemyLight.generated.h"

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API AAI_BookEnemyLight : public AAI_RangeEnemy
{
	GENERATED_BODY()
public:
	AAI_BookEnemyLight(const class FObjectInitializer& PCIP);
};
