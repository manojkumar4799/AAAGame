// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Characeter/GameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characeter/EchoAnimInstance.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	echoCharacter = Cast<AGameCharacter>(TryGetPawnOwner());
	if (echoCharacter)
	{
		charMovementComp = echoCharacter->GetCharacterMovement();
	}

}

void UEchoAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);
	if (charMovementComp) 
	{
		groundSpeed = UKismetMathLibrary::VSizeXY(charMovementComp->Velocity);
		isFalling= charMovementComp->IsFalling();
		characterState= echoCharacter->GetCharacterState();
	}
}
