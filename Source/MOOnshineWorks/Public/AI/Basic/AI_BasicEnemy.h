// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "AI_BasicController.h"
#include "Materials/Material.h"
#include "AI_BasicEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AAI_BasicEnemy*, Enemy);

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API AAI_BasicEnemy : public ACharacter
{
	GENERATED_BODY()
public:
	AAI_BasicEnemy(const class FObjectInitializer& PCIP);

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;

	/** Pawn sensing Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness)
	UPawnSensingComponent* PawnSensor;

//	UFUNCTION()
//	void PostInitializeComponents();
	
	UFUNCTION(BlueprintCallable, Category = Items)
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

	UFUNCTION(BlueprintCallable, Category = Items)
	void OnSeePawn(APawn *OtherPawn);

	/** Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	float Health;

	/** Defense */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	float Defense;

	/** Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	float WalkSpeed;

	/** Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	float Damage;

	/** Check if AI is active, else he cant be hit/killed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	bool CanBeHit;

	/** Should Patrol or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStats)
	bool AIPatrol;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* Behavior;

	void StartSprint();

	void StartWalk();

    UFUNCTION(BlueprintNativeEvent, Category = AIStats)
    void OnDealDamage();
    
	UFUNCTION(BlueprintCallable, Category = AIStats)
	void DealDamage(float DamageInflicted);
	
	UFUNCTION(BlueprintCallable, Category = MOOnshine)
	void AddImpulseToEnemy(FVector Impulse);

	UFUNCTION(BlueprintCallable, Category = AIStats)
	void Die();

	UFUNCTION(BlueprintNativeEvent, Category = MOO)
	void OnDie();

	UFUNCTION(BlueprintCallable, Category = MOO)
	void AIBecameActive();

	UFUNCTION(BlueprintNativeEvent, Category = MOO)
	void OnAIBecameActive();

	UFUNCTION(BlueprintCallable, Category = MOO)
	FName GetPersistentId();
	UFUNCTION(BlueprintCallable, Category = MOO)
	void SetPersistentId(FName NewPersistentId);

	UFUNCTION(BlueprintCallable, Category = MOO)
	void Save(bool StopSpawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* TheMaterial;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	//UMaterialInterface* Base_Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActivateEnemy)
	TSubclassOf<AActor> DeathBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MOOnshine)
	TSubclassOf<AAI_BasicEnemy> DestructibleEnemyClass;

private:
	FName PersistentId;

protected:
	virtual void ReceiveBeginPlay() override;
};
