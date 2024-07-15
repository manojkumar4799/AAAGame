// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Attributes/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

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

	attributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthComponet = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthComponent"));
	HealthComponet->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()

{
	Super::BeginPlay();
	if (HealthComponet) HealthComponet->SetVisibility(false);

	enemyController = Cast<AAIController>(GetController());
	patrolTarget = patroltargetPoints[FMath::RandRange(0, patroltargetPoints.Num() - 1)];
	MoveToTarget(patrolTarget);
	

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

	if (combatTarget) {
		const double distanceToTarget = (combatTarget->GetActorLocation() - GetActorLocation()).Size();

		if( !IstargetInRadius(combatTarget,triggerRadius)) {
			combatTarget = nullptr;
			if (HealthComponet) HealthComponet->SetVisibility(false);
		}
	}

	Patrol();

}

void AEnemy::MoveToTarget( AActor* target)
{
	if (enemyController) {
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(target);
		moveRequest.SetAcceptanceRadius(10.f);
		moveRequest.SetReachTestIncludesAgentRadius(false);
		FNavPathSharedPtr navPath;
		enemyController->MoveTo(moveRequest, &navPath);
		TArray<FNavPathPoint> pathPoints = navPath->GetPathPoints();

		if (pathPoints.Num() > 0) {
			for (auto point : pathPoints) {
				DrawDebugSphere(GetWorld(), point.Location, 10.f, 20.F, FColor::Green, true);
			}
		}

	}
}

void AEnemy::Patrol()
{
	if (patrolTarget) {

		TArray<AActor*> validTargetPoints;

		for (AActor* validTarget : patroltargetPoints) {

			if (validTarget != patrolTarget) {
				validTargetPoints.AddUnique(validTarget);
			}

		}

		if (IstargetInRadius(patrolTarget, patrolPointRadius)) {

			if (validTargetPoints.Num() > 0) {
				int32 randomPatrolPointIndex = FMath::RandRange(0, validTargetPoints.Num() - 1);
				AActor* randomPatrolPoint = validTargetPoints[randomPatrolPointIndex];
				patrolTarget = randomPatrolPoint;

				MoveToTarget(patrolTarget);
			}

		}
	}
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
	
	if (HealthComponet) HealthComponet->SetVisibility(true);
	if(hitVFX)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitVFX, hitImpactPoint);

	// forward * toHit =|forward| |toHit| CosTheta
	 double CosTheta= FVector::DotProduct(forwardVector, toHitPoint);
	 FVector crossProduct= FVector::CrossProduct(forwardVector, toHitPoint);
	 double radains= FMath::Acos(CosTheta);
	 double degrees = FMath::RadiansToDegrees(radains);

	 if (crossProduct.Z < 0) {
		 degrees *= -1;
	 }

	 if (attributeComp->IsAlive()) PlayHitReaction(degrees);
	 else PlayDeathMontage();

	 UGameplayStatics::PlaySoundAtLocation(this, hitSound, hitImpactPoint);

	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct.GetSafeNormal() * 70,7,FLinearColor::Green, 5,2);
	 if (GEngine) {
		 GEngine->AddOnScreenDebugMessage(1, 5, FColor::Black, FString::Printf(TEXT("Degrees: %f"), degrees));
	 }
	 

	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 70, 7, FLinearColor::Yellow, 5,2);
	 UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHitPoint * 70, 7, FLinearColor::Red, 5,2);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attributeComp && HealthComponet) {
		attributeComp->Receivedamage(DamageAmount);
		HealthComponet->SetHealthPercent(attributeComp->GetHealthPercent());
		combatTarget = EventInstigator->GetPawn();
	}
	
	return DamageAmount;
}

void AEnemy::PlayHitReaction(double angle)
{
	FName sectionToPlay = FName("FromBack");
	if (angle <= 45 && angle > -45) sectionToPlay = FName("FromFront");
	else if (angle <= -45 && angle > -135) sectionToPlay = FName("FromLeft");
	else if (angle > 45 && angle <= 135) sectionToPlay = FName("FromRight");
	PlayHitReactionMontage(sectionToPlay);
}

void AEnemy::PlayDeathMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (HealthComponet) HealthComponet->SetVisibility(false);
	if (animInstance) {
		animInstance->Montage_Play(deathMontage);
		TArray<FName> selection;
		selection.Add("Death1");
		selection.Add("Death2");
		selection.Add("Death3");
		selection.Add("Death4");
		int32 random = FMath::RandRange(0, selection.Num() - 1);
		deathStatus = static_cast<EDeathStatus>(random+1);
		animInstance->Montage_JumpToSection(selection[random],deathMontage);

	}
}

bool AEnemy::IstargetInRadius(AActor* targetActor, double radius)
{
	double distance = (targetActor->GetActorLocation() - GetActorLocation()).Size();
	DrawDebugSphere(GetWorld(), targetActor->GetActorLocation(), 30.f, 20.F, FColor::Yellow);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 30.f, 20.F, FColor::Yellow);
	return distance <= radius;
}

