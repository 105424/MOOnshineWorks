// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API AMenuPawn : public APawn
{
	GENERATED_BODY()
public:
	AMenuPawn(const class FObjectInitializer& PCIP);

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = "MOOnshine")
	AActor* getPointedObject();

	UFUNCTION(BlueprintCallable, Category = "MOOnshine")
	void Click();

protected:

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};
