// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Characeter/CharacterTypes.h"
#include "Enemy.generated.h"



UCLASS()
class GAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayHitReactionMontage(const FName& sectionName);

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* hitReactionMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* deathMontage;

	UPROPERTY(BlueprintReadOnly)
	EDeathStatus  deathStatus = EDeathStatus::EDS_Alive;

	UPROPERTY(EditAnywhere)
	float triggerRadius = 600;

	//Navigation

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& hitImpactPoint);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void PlayHitReaction(double angle);

	void PlayDeathMontage();

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* hitVFX;

	UPROPERTY(EditAnywhere)
	class UAttributeComponent* attributeComp;

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthComponet;


	//Navigation
	bool IstargetInRadius(AActor* targetActor, double radius);
	void MoveToTarget(AActor* target);

	void Patrol();

	




};
