// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 

#include "Characeter/BaseCharacter.h"
#include "Enemy.generated.h"



UCLASS()
class GAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;	

	
	/*Attack*/
	void AttackEnd();
	
	bool IsEngaged();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> weaponClass;
	/*Attack*/
	

	/*AI*/
	
	UPROPERTY(EditAnywhere)
	float chaseRadius = 600;

	UPROPERTY(EditAnywhere)
	float attackRadius = 150;

	UPROPERTY(BlueprintReadOnly)
	AActor* combatTarget = nullptr;

	
	AActor* patrolTarget;

	UPROPERTY(EditInstanceOnly)
	double patrolPointRadius = 100;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> patroltargetPoints;
	
	class AAIController* enemyController;

	UPROPERTY(EditAnywhere)
	float acceptanceRadius= 30.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState currentEnemyState = EEnemyState::EES_Patrolling;
	/*AI*/


	

	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	/*IHitInterface*/
	virtual void GetHit_Implementation(const FVector& hitImpactPoint, AActor* hitter);
	/*IHitInterface*/

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	

private:
	

	void OnDeath() override;


	/*AI*/
	UFUNCTION()
	void OnPawnSeen(APawn* seenPawn);
	void Attack();
	void StartPatrol();
	void LoseInterestTowardsPlayer();
	bool IsOutofChaseRadius();
	void StartChasing();
	bool IsOutOfAttackRadius();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	void StartAttackTimer();
	void ClearTimer(FTimerHandle timer);
	bool IsDead();		
	bool IstargetInRadius(AActor* targetActor, double radius);
	void MoveToTarget(AActor* target);
	void PatrolCheck();
	void CombatCheck();
	void PatrolTimerFinished();	
	

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* pawnSense;
	/*AI*/


	/*Timers*/
	FTimerHandle AttackTimer;
	FTimerHandle patrolTimer;
	/*Timers*/

	/*Health*/
	void HandleHealthBar(bool bShoulbBeVisible);

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthComponet;
	/*Health*/


};
