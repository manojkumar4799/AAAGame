// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characeter/CharacterTypes.h"
#include "GameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AActorItem;
class AWeapon;



UCLASS()
class GAME_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EKeyPressed();
	void AttackWithSword();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void EquipSword();

	UFUNCTION(BlueprintCallable)
	void Unarm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float value);
	void Turn(float value);
	void LookUp(float value);
	void MoveRight(float value);
	void PlayAttackMontage();
	void PlayEquipMontage(FName sectionName);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* cameraArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* hair;

	UPROPERTY(VisibleInstanceOnly)
	AActorItem* overlapingItem;

	

	ECharacterState characterState = ECharacterState::ECS_Unequiped;

	UPROPERTY(EditDefaultsOnly, Category = "Anim Monatge")
	UAnimMontage* echoAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim Monatge")
	UAnimMontage* echoEquipMontage;


	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	EActionState actionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	AWeapon* currentWeapon;

	

public:
	FORCEINLINE  void SetOverlapingItem(AActorItem* item) { overlapingItem = item; }

	FORCEINLINE ECharacterState GetCharacterState() { return characterState; }
	

};
