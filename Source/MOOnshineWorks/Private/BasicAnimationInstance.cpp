// Fill out your copyright notice in the Description page of Project Settings.

#include "MOOnshineWorks.h"
#include "BasicAnimationInstance.h"

UBasicAnimationInstance::UBasicAnimationInstance(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	AIIdle = false;
	AIPatrolling = false;
	AIAttacking = false;
	AIDeath = false;
	AICharging = false;
}


