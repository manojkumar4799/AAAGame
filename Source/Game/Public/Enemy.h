// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "Characeter/CharacterTypes.h"
#include "Characeter/BaseCharacter.h"
#include "Enemy.generated.h"



UCLASS()
class GAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//BaseClass

	

	UPROPERTY(BlueprintReadOnly)
	EDeathStatus  deathStatus = EDeathStatus::EDS_Alive;

	

	//Navigation
	UPROPERTY(EditAnywhere)
	float chaseRadius = 600;

	UPROPERTY(EditAnywhere)
	float attackRadius = 150;

	UPROPERTY()
	AActor* combatTarget = nullptr;

	UPROPERTY(EditInstanceOnly)
	AActor* patrolTarget;

	UPROPERTY(EditInstanceOnly)
	double patrolPointRadius = 15;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> patroltargetPoints;

	//Controllers
	class AAIController* enemyController;

	UPROPERTY(EditAnywhere)
	float acceptanceRadius= 30.f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& hitImpactPoint);//BaseClass
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	


private:
	

	void OnDeath();//BaseClass

	UFUNCTION()
	void OnPawnSeen(APawn* seenPawn);

	

//BaseClass

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthComponet;


	//Navigation
	bool IstargetInRadius(AActor* targetActor, double radius);
	void MoveToTarget(AActor* target);

	void PatrolCheck();
	void CombatCheck();

	void PatrolTimerFinished();

	

	FTimerHandle patrolTimer;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* pawnSense;

	

	EEnemyState currentEnemyState = EEnemyState::EES_Patrolling;


};
