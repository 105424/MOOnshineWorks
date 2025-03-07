// Fill out your copyright notice in the Description page of Project Settings.

#include "MOOnshineWorks.h"
#include "AI_BasicController.h"
#include "AI_BasicEnemy.h"
#include "MOOnshineWorksCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BasicAnimationInstance.h"
#include "AI_BookEnemyLight.h"
#include "AI_PegEnemyDark.h"
#include "AI_BarrelEnemy.h"
#include "AI_PianoEnemy.h"

AAI_BasicController::AAI_BasicController(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	BlackboardComp = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));
	BehaviorComp = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void AAI_BasicController::Possess(class APawn *InPawn)
{
	Super::Possess(InPawn);
	AAI_BasicEnemy* BaseChar = Cast<AAI_BasicEnemy>(InPawn);
	if (BaseChar && BaseChar->Behavior)
	{
		BlackboardComp->InitializeBlackboard(*BaseChar->Behavior->BlackboardAsset);
		StateAI = BlackboardComp->GetKeyID("StateAI");
		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyLocationID = BlackboardComp->GetKeyID("Destination");
		EnemyDistance = BlackboardComp->GetKeyID("EnemyDistance");
		SetPatrolRoute = BlackboardComp->GetKeyID("PatrolTo");
		WhereShouldAIPatrolTo = BlackboardComp->GetKeyID("WhereShouldAIPatrolTo");
		OriginalPosition = BlackboardComp->GetKeyID("OriginalPosition");
		GotEnemyAsTarget = BlackboardComp->GetKeyID("GotEnemyInSight/Hear");
		LastSeenPosition = BlackboardComp->GetKeyID("LastSeenPosition");
		ShouldTheAIPatrol = BlackboardComp->GetKeyID("ShouldTheAIPatrol");
		AIUsedForTrap = BlackboardComp->GetKeyID("AIUsedForTrap");
		EnemyDistanceShouldAttack = BlackboardComp->GetKeyID("EnemyDistanceShouldAttack");
		ChargePosition = BlackboardComp->GetKeyID("ChargePosition");

		BehaviorComp->StartTree(*BaseChar->Behavior);
	}

	SetOriginalPosition();
}

void AAI_BasicController::SetOriginalPosition()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}
	const FVector MyLoc = MyBot->GetActorLocation();
	BlackboardComp->SetValueAsVector(OriginalPosition, MyLoc);
}

void AAI_BasicController::SearchForEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AMOOnshineWorksCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		AMOOnshineWorksCharacter* TestPawn = Cast<AMOOnshineWorksCharacter>(*It);
		if (TestPawn)
		{
			const float DistSq = FVector::Dist(TestPawn->GetActorLocation(), MyLoc);
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}

	if (BestPawn)
	{
		BlackboardComp->SetValueAsFloat(EnemyDistance, BestDistSq);
		//BlackboardComp->SetValueAsFloat(WhereShouldAIPatrolTo, BestDistSq);
		SetEnemy(BestPawn);
	}
}
void AAI_BasicController::SetEnemy(class APawn *InPawn)
{
	BlackboardComp->SetValueAsObject(EnemyKeyID, InPawn);
	BlackboardComp->SetValueAsVector(EnemyLocationID, InPawn->GetActorLocation());
}

void AAI_BasicController::FoundPlayer() //Bool in blackboard setten voor behaviour tree
{
	BlackboardComp->SetValueAsBool(GotEnemyAsTarget, true);
}

void AAI_BasicController::ResetSight()
{
	BlackboardComp->SetValueAsBool(GotEnemyAsTarget, false);
}

void AAI_BasicController::LostPlayer() //Bool in blackboard setten voor behaviour tree en reset patrol key
{
	BlackboardComp->SetValueAsBool(GotEnemyAsTarget, false);
	BlackboardComp->SetValueAsFloat(WhereShouldAIPatrolTo, 0);

	//Pak positie speler en set deze om daar nog heen te lopen!
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector MyLoc = GetPawn()->GetActorLocation();
	if (Player)
	{
		MyLoc = Player->GetActorLocation();
	}

	BlackboardComp->SetValueAsVector(LastSeenPosition, MyLoc);
}

void AAI_BasicController::SetAttackAnimation()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	BasicAnimInstance->AIAttacking = true;
	BasicAnimInstance->AIPatrolling = false;
	BasicAnimInstance->AIIdle = false;
	//BasicAnimInstance->Jumping = false;
}

void AAI_BasicController::SetIdleAnimation()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	BasicAnimInstance->AIAttacking = false;
	BasicAnimInstance->AIPatrolling = false;
	BasicAnimInstance->AIIdle = true;
	BasicAnimInstance->AICharging = false;
}

void AAI_BasicController::SetPatrollingAnimation()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	BasicAnimInstance->AIAttacking = false;
	BasicAnimInstance->AIPatrolling = true;
	BasicAnimInstance->AIIdle = false;
	//BasicAnimInstance->Jumping = false;
}
void AAI_BasicController::SetChargeAnimation()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	BasicAnimInstance->AIAttacking = false;
	BasicAnimInstance->AIPatrolling = false;
	BasicAnimInstance->AIIdle = false;
	BasicAnimInstance->AICharging = true;
}

void AAI_BasicController::SetJumpingAnimation()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	BasicAnimInstance->AIAttacking = false;
	BasicAnimInstance->AIPatrolling = false;
	BasicAnimInstance->AIIdle = false;
	//BasicAnimInstance->Jumping = true;
}

void AAI_BasicController::SetSpeedAnimation(float speed)
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
	APawn* Inst = Controller->GetPawn();
	USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();
	UBasicAnimationInstance* BasicAnimInstance = (UBasicAnimationInstance*)MeshComponent->GetAnimInstance();

	//BasicAnimInstance->Speed = speed;
}
void AAI_BasicController::AISetPatrolState()
{
	if (this != NULL)
	{ 
		int State = 0;
		BlackboardComp->SetValueAsInt(StateAI, State);
		BlackboardComp->SetValueAsBool(GotEnemyAsTarget, false);
	}
}

void AAI_BasicController::AISetAttackState()
{
	if (this != NULL)
	{
		int State = 1;
		BlackboardComp->SetValueAsInt(StateAI, State);
	}
}

void AAI_BasicController::AISetSearchState()
{
	if (this != NULL)
	{
		int State = 2;
		BlackboardComp->SetValueAsInt(StateAI, State);
	}
}
void AAI_BasicController::ShouldAIPatrol()
{
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	if (BasicEnemy != NULL)
	{
		if (BasicEnemy->AIPatrol == false) //AI moet niet patrollen en State is niet al gezet naar 3
		{
			BlackboardComp->SetValueAsBool(ShouldTheAIPatrol, false);
		}
		if (BasicEnemy->AIPatrol == true)
		{
			BlackboardComp->SetValueAsBool(ShouldTheAIPatrol, true);
		}
	}
}

void AAI_BasicController::ActivateEnemy()
{
	//Make eyes of the new enemy go red!
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	if (BasicEnemy)
	{ 
		USkeletalMeshComponent* MeshComponent = BasicEnemy->GetMesh();

		MeshComponent->SetMaterial(1, BasicEnemy->TheMaterial);
	}
}
/*
void AAI_BasicController::SetEnemyDistanceShouldAttack() 
{
	//Zet de EnemyDistance in de behaviour tree!
	AAI_BasicEnemy* BasicEnemy = (AAI_BasicEnemy*)GetPawn();
	if (BasicEnemy)
	{
		AAI_BasicController* Controller = (AAI_BasicController*)BasicEnemy->GetController();
		BlackboardComp->SetValueAsFloat(EnemyDistanceShouldAttack, BasicEnemy->EnemyDistanceShouldAttack);
	}
}
*/
void AAI_BasicController::CalculateChargePosition()
{
	AAI_BasicEnemy* BasicEnemy = Cast<AAI_BasicEnemy>(GetPawn());
	AAI_ChargeEnemy* ChargeEnemy = Cast<AAI_ChargeEnemy>(GetPawn());
	AMOOnshineWorksCharacter* playerCharacter = (AMOOnshineWorksCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (BasicEnemy && playerCharacter)
	{ 
		//Snelheid van de AI omhoog tijdens charge!
		BasicEnemy->GetCharacterMovement()->MaxWalkSpeed = ChargeEnemy->ChargeSpeed;
		//BasicEnemy->MaxWalkSpeed = BasicEnemy->ChargeSpeed;

		//Bereken charge locatie!
		FVector CalculateDifferentLocation = BasicEnemy->GetActorLocation()-playerCharacter->GetActorLocation();
		FVector chargelocation = playerCharacter->GetActorLocation()-(0.6*CalculateDifferentLocation);
		BlackboardComp->SetValueAsVector(ChargePosition, chargelocation);
	}
}
void AAI_BasicController::AttackPlayer()
{
	//override, minder services nodig
}
void AAI_BasicController::Patrol()
{
	//override, minder services nodig
}
AAI_BasicEnemy* AAI_BasicController::GoActive()
{
	return nullptr;
	//override, minder services nodig
}
void AAI_BasicController::SetMovementSpeed()
{
	AAI_BasicEnemy* BasicEnemy = Cast<AAI_BasicEnemy>(GetPawn());
	BasicEnemy->GetCharacterMovement()->MaxWalkSpeed = BasicEnemy->WalkSpeed;
}

