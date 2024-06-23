// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetHit(const FVector& hitImpactPoint);

private:
	void PlayHitReaction(double angle);

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

};
