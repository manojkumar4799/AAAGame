// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characeter/CharacterTypes.h"
#include "EchoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UEchoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AGameCharacter* echoCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* charMovementComp;

	UPROPERTY(BlueprintReadOnly)
	float groundSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool isFalling;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState characterState;

	

	
};
