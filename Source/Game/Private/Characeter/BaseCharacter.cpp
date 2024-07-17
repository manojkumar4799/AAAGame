// Fill out your copyright notice in the Description page of Project Settings.


#include "Characeter/BaseCharacter.h"
#include "Components/BoxComponent.h"

#include "Attributes/AttributeComponent.h"
#include "weapons/Weapon.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	attributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}


//handle the weapon collision while attacking
void ABaseCharacter::HandleCollisionForWeaponBoxCollider(ECollisionEnabled::Type Collisiontype)
{
	equipWeapon->GetBoxCollider()->SetCollisionEnabled(Collisiontype);
	if (Collisiontype == ECollisionEnabled::QueryOnly) {
		equipWeapon->ignoreActor = nullptr;
	}

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::OnDeath()
{
}

void ABaseCharacter::PlayHitReactionMontage(const FName& sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(hitReactionMontage);
		animInstance->Montage_JumpToSection(sectionName, hitReactionMontage);
	}
}

void ABaseCharacter::PlayHitReaction(double angle)
{
	FName sectionToPlay = FName("FromBack");
	if (angle <= 45 && angle > -45) sectionToPlay = FName("FromFront");
	else if (angle <= -45 && angle > -135) sectionToPlay = FName("FromLeft");
	else if (angle > 45 && angle <= 135) sectionToPlay = FName("FromRight");
	PlayHitReactionMontage(sectionToPlay);
}



