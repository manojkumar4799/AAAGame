// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characeter/GameCharacter.h"
#include "Components/BoxComponent.h"
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

		weapon->Equip(GetMesh(), FName("RightHandSocket"),this, this);
		characterState = ECharacterState::ECS_Equiped;
		equipWeapon = weapon;
		overlapingItem = nullptr;
	}
	else if (characterState == ECharacterState::ECS_Equiped && actionState == EActionState::EAS_Unoccupied) {

		PlayEquipMontage(FName("Unequip"));
		characterState = ECharacterState::ECS_Unequiped;
		actionState = EActionState::EAS_Equipping;
	}
	else if (equipWeapon && characterState == ECharacterState::ECS_Unequiped && actionState == EActionState::EAS_Unoccupied) {
		PlayEquipMontage(FName("Equip"));
		characterState = ECharacterState::ECS_Equiped;
		actionState = EActionState::EAS_Equipping;
	}
}

void AGameCharacter::AttackWithSword()
{
	if (actionState == EActionState::EAS_Unoccupied && characterState== ECharacterState::ECS_Equiped) {
		PlayAttackMontage();
		actionState = EActionState::EAS_Attacking;
	}
	

}

void AGameCharacter::AttackEnd()
{
	actionState = EActionState::EAS_Unoccupied;
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
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AGameCharacter::AttackWithSword);

}

void AGameCharacter::MoveForward(float value)
{
	if (actionState != EActionState::EAS_Unoccupied) return;
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
	if (actionState != EActionState::EAS_Unoccupied) return;
	if (Controller && value != 0) {
		
		FRotator controllerRotation = GetControlRotation();
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);

		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}	
}

void AGameCharacter::PlayAttackMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (animInstance && echoAttackMontage) {

		animInstance->Montage_Play(echoAttackMontage);
		int32 sectionToPlay = FMath::RandRange(0, 1);
		FName sectionName = FName();

		switch (sectionToPlay)
		{
		case 0:
			sectionName = FName("Attack1");
			break;

		default:
			sectionName = FName("Attack2");
			break;
		}
		sectionName = FName("Attack1");
		animInstance->Montage_JumpToSection(sectionName, echoAttackMontage);
	}
}

void AGameCharacter::PlayEquipMontage(FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance) return;
	animInstance->Montage_Play(echoEquipMontage);
	animInstance->Montage_JumpToSection(sectionName, echoEquipMontage);
	
}

//called from notifier and blueprint
void AGameCharacter::EquipSword()
{
	equipWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
}

//called from notifier and blueprint
void AGameCharacter::Unarm()
{
	equipWeapon->AttachMeshToSocket(GetMesh(), FName("WeaponSocket"));
}

//called from notifier and blueprint
void AGameCharacter::FinishEquipping()
{
	actionState = EActionState::EAS_Unoccupied;
}

void AGameCharacter::HandleCollisionForBoxCollider(ECollisionEnabled::Type Collisiontype)
{
	equipWeapon->GetBoxCollider()->SetCollisionEnabled(Collisiontype);
	if (Collisiontype == ECollisionEnabled::QueryOnly) {
		equipWeapon->ignoreActor = nullptr;
	}
}