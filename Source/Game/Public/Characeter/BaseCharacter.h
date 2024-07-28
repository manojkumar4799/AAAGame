// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "Characeter/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"



UCLASS()
class GAME_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*IHitInterface*/
	virtual void GetHit_Implementation(const FVector& hitImpactPoint , AActor* hitter) override;
	/*IHitInterface*/
	double DebugHitPositions(const FVector& hitImpactPoint);

	/*Attack*/
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();


	virtual void Attack();
	virtual void PlayHitSound(const FVector& hitImpactPoint);
	virtual void PlayHitVFX(const FVector& hitImpactPoint);

	class AWeapon* equipWeapon;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* hitVFX;
	
	

	/*Monatage*/
	void PlayAttackMontage();
	void PlayDeathMontage();
	void StopPlayingMontage(UAnimMontage* montage);
	void PlayMontageSection(UAnimMontage* montage, const FName sectionName);	
	
	void PlayHitReactionMontage(const FName& sectionName);	void PlayHitReaction(double angle);

	

	
	UPROPERTY(EditDefaultsOnly, Category = "Anim Monatge")
	UAnimMontage* attackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* hitReactionMontage;
	UPROPERTY(EditAnywhere)
	class UAttributeComponent* attributeComp;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* deathMontage;
	

	UPROPERTY(EditAnywhere)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere)
	TArray<FName> DeathMontageSections;


	/*Health*/
	virtual void OnDeath();
	UPROPERTY(BlueprintReadOnly)
	EDeathPose  deathPose = EDeathPose::EDP_DeathPose1;

public:
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	void HandleCollisionForWeaponBoxCollider(ECollisionEnabled::Type Collisiontype);

};
