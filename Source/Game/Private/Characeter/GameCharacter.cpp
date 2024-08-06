// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characeter/GameCharacter.h"
#include "Components/BoxComponent.h"
#include "weapons/Weapon.h"

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

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

}

float AGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EngageableTarget"));
	
}

void AGameCharacter::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(overlapingItem);
	if (weapon) {

		EquipWeapon(weapon);
	}
	else if (characterState == ECharacterState::ECS_Equiped && actionState == EActionState::EAS_Unoccupied) {

		UnequipWeapon();
	}
	else if (equipWeapon && characterState == ECharacterState::ECS_Unequiped && actionState == EActionState::EAS_Unoccupied) {
		EquipWeaponFromback();
	}
}

void AGameCharacter::Attack()
{
	if (CanAttack()) {

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
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AGameCharacter::Attack);

}

void AGameCharacter::GetHit_Implementation(const FVector& hitImpactPoint, AActor* hitter)
{
	Super::GetHit_Implementation(hitImpactPoint, hitter);
	actionState = EActionState::EAS_HitReact;
	HandleCollisionForWeaponBoxCollider(ECollisionEnabled::NoCollision);
	DrawDebugSphere(GetWorld(), hitImpactPoint, 10.f, 16, FColor::Black, false, 3.f);
	UE_LOG(LogTemp, Warning, TEXT("GetHitFunction "));


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



void AGameCharacter::PlayEquipMontage(FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance) return;
	animInstance->Montage_Play(echoEquipMontage);
	animInstance->Montage_JumpToSection(sectionName, echoEquipMontage);
	
}

void AGameCharacter::EquipWeapon(AWeapon* weapon)
{
	weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	characterState = ECharacterState::ECS_Equiped;
	equipWeapon = weapon;
	overlapingItem = nullptr;
}

void AGameCharacter::UnequipWeapon()
{
	PlayEquipMontage(FName("Unequip"));
	characterState = ECharacterState::ECS_Unequiped;
	actionState = EActionState::EAS_Equipping;
}

void AGameCharacter::EquipWeaponFromback()
{
	PlayEquipMontage(FName("Equip"));
	characterState = ECharacterState::ECS_Equiped;
	actionState = EActionState::EAS_Equipping;
}

bool AGameCharacter::CanAttack()
{
	return actionState == EActionState::EAS_Unoccupied && characterState == ECharacterState::ECS_Equiped;
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

void AGameCharacter::HiReactEnd()
{
	actionState = EActionState::EAS_Unoccupied;
}

