// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"



UCLASS()
class GAME_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() ;
	virtual void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual void OnDeath();
	void PlayHitReactionMontage(const FName& sectionName);
	void PlayHitReaction(double angle);


	class AWeapon* equipWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Anim Monatge")
	UAnimMontage* echoAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* hitReactionMontage;
	UPROPERTY(EditAnywhere)
	class UAttributeComponent* attributeComp;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* deathMontage;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* hitVFX;


public:
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	void HandleCollisionForWeaponBoxCollider(ECollisionEnabled::Type Collisiontype);

};
