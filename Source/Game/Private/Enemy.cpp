// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactionMontage(const FName& sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(hitReactionMontage);
		animInstance->Montage_JumpToSection(sectionName, hitReactionMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& hitImpactPoint)
{
	DrawDebugSphere( GetWorld(), hitImpactPoint, 10.f, 16, FColor::Black, false, 3.f);
	UE_LOG(LogTemp, Warning, TEXT("GEtHotFunction "));

	const FVector forwardVector = GetActorForwardVector();

	const FVector LowerHitPoint(hitImpactPoint.X, hitImpactPoint.Y, GetActorLocation().Z);
	const FVector toHitPoint = (LowerHitPoint - GetActorLocation()).GetSafeNormal();
	
	if(hitVFX)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitVFX, hitImpactPoint);

	// forward * toHit =|forward| |toHit| CosTheta
	 double CosTheta= FVector::DotProduct(forwardVector, toHitPoint);
	 FVector crossProduct= FVector::CrossProduct(forwardVector, toHitPoint);
	 double radains= FMath::Acos(CosTheta);
	 double degrees = FMath::RadiansToDegrees(radains);

	 if (crossProduct.Z < 0) {
		 degrees *= -1;
	 }
	 PlayHitReaction(degrees);

	 UGameplayStatics::PlaySoundAtLocation(this, hitSound, hitImpactPoint);

	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct.GetSafeNormal() * 70,7,FLinearColor::Green, 5,2);
	 if (GEngine) {
		 GEngine->AddOnScreenDebugMessage(1, 5, FColor::Black, FString::Printf(TEXT("Degrees: %f"), degrees));
	 }

	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 70, 7, FLinearColor::Yellow, 5,2);
	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHitPoint * 70, 7, FLinearColor::Red, 5,2);
}

void AEnemy::PlayHitReaction(double angle)
{
	FName sectionToPlay = FName("FromBack");
	if (angle <= 45 && angle > -45) sectionToPlay = FName("FromFront");
	else if (angle <= -45 && angle > -135) sectionToPlay = FName("FromLeft");
	else if (angle > 45 && angle <= 135) sectionToPlay = FName("FromRight");
	PlayHitReactionMontage(sectionToPlay);
}

