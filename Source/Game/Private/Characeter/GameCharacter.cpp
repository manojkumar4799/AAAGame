// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characeter/GameCharacter.h"
#include "weapons/Weapon.h"


// Sets default values
AGameCharacter::AGameCharacter()
{

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	cameraArm->SetupAttachment(GetRootComponent());

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraArm);

	hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	hair->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameCharacter::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(overlapingItem);
	if (weapon) {

		weapon->Equip(GetMesh(), FName("RightHandSocket"));
		characterState = ECharacterState::ECS_Equiped;
	}
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AGameCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGameCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AGameCharacter::EKeyPressed);

}

void AGameCharacter::MoveForward(float value)
{
	if (Controller && value != 0)
	{

		FRotator controllerRotation = GetControlRotation();
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);
		FVector directionVector= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(directionVector, value);
	}
}

void AGameCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void AGameCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AGameCharacter::MoveRight(float value)
{
	if (Controller && value != 0) {
		
		FRotator controllerRotation = GetControlRotation();
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);

		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}	
}

