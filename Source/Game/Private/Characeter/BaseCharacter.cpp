// Fill out your copyright notice in the Description page of Project Settings.


#include "Characeter/BaseCharacter.h"
#include "Components/BoxComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

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

void ABaseCharacter::GetHit_Implementation(const FVector& hitImpactPoint, AActor* hitter)
{
	PlayHitVFX(hitImpactPoint);
	PlayHitSound(hitImpactPoint);
	if (attributeComp->IsAlive()) PlayHitReaction(DebugHitPositions(hitter->GetActorLocation()));
	else OnDeath();
}

double ABaseCharacter::DebugHitPositions(const FVector& hitImpactPoint)
{
	const FVector forwardVector = GetActorForwardVector();

	const FVector LowerHitPoint(hitImpactPoint.X, hitImpactPoint.Y, GetActorLocation().Z);
	const FVector toHitPoint = (LowerHitPoint - GetActorLocation()).GetSafeNormal();

	/** ForDebugging*/
	// forward * toHit =|forward| |toHit| CosTheta
	double CosTheta = FVector::DotProduct(forwardVector, toHitPoint);
	FVector crossProduct = FVector::CrossProduct(forwardVector, toHitPoint);
	double radains = FMath::Acos(CosTheta);
	double degrees = FMath::RadiansToDegrees(radains);

	if (crossProduct.Z < 0) {
		degrees *= -1;
	}

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct.GetSafeNormal() * 70, 7, FLinearColor::Green, 5, 2);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Black, FString::Printf(TEXT("Degrees: %f"), degrees));
	}


	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 70, 7, FLinearColor::Yellow, 5, 2);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHitPoint * 70, 7, FLinearColor::Red, 5, 2);
	return degrees;
}


void ABaseCharacter::Attack()
{
}

void ABaseCharacter::PlayAttackMontage()
{
	if (AttackMontageSections.Num() > 0 ) {
		int32 randomSelectionIndex = FMath::RandRange(0, AttackMontageSections.Num() - 1);
		PlayMontageSection(attackMontage, AttackMontageSections[randomSelectionIndex]);
	}
}

void ABaseCharacter::PlayDeathMontage()
{
	if (DeathMontageSections.Num() > 0) {
		int32 randomSelectionIndex = FMath::RandRange(0, DeathMontageSections.Num() - 1);
		TEnumAsByte<EDeathPose> Pose(randomSelectionIndex);
		deathPose = Pose;
		PlayMontageSection(deathMontage, DeathMontageSections[randomSelectionIndex]);
	}
}

void ABaseCharacter::StopPlayingMontage(UAnimMontage* montage)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Stop(0.15f, montage);
	}
}



void ABaseCharacter::PlayMontageSection(UAnimMontage* montage, const FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && montage) {
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName, montage);
	}
}

void ABaseCharacter::AttackEnd()
{

}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (attributeComp) {
		attributeComp->Receivedamage(DamageAmount);
	}
}

void ABaseCharacter::OnDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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


void ABaseCharacter::PlayHitSound(const FVector& hitImpactPoint)
{
	if (hitSound) UGameplayStatics::PlaySoundAtLocation(this, hitSound, hitImpactPoint);
}

void ABaseCharacter::PlayHitVFX(const FVector& hitImpactPoint)
{
	const FVector forwardVector = GetActorForwardVector();

	const FVector LowerHitPoint(hitImpactPoint.X, hitImpactPoint.Y, GetActorLocation().Z);
	const FVector toHitPoint = (LowerHitPoint - GetActorLocation()).GetSafeNormal();


	if (hitVFX)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitVFX, hitImpactPoint);
}



