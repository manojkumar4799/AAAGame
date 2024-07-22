// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "weapons/Weapon.h"
#include "Attributes/AttributeComponent.h"

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

	

	HealthComponet = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthComponent"));
	HealthComponet->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	pawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	pawnSense->SightRadius = 4000.f;
	pawnSense->SetPeripheralVisionAngle(75.f);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()

{
	Super::BeginPlay();
	if (HealthComponet) HealthComponet->SetVisibility(false);

	enemyController = Cast<AAIController>(GetController());
	patrolTarget = patroltargetPoints[FMath::RandRange(0, patroltargetPoints.Num() - 1)];
	MoveToTarget(patrolTarget);
	if (pawnSense) {
		pawnSense->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	}

	if (weaponClass) {
		equipWeapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		equipWeapon->Equip(GetMesh(), FName("WeaponRightHandSocket"), this, this);
		
	}
}

void AEnemy::AttackEnd()
{
    Super::AttackEnd();
	currentEnemyState = EEnemyState::EES_NoState;
}

bool AEnemy::IsEngaged()
{
	return currentEnemyState==EEnemyState::EES_Engaged;
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) {
		return;
	}

	if(currentEnemyState== EEnemyState::EES_Patrolling)	PatrolCheck();
	else CombatCheck();
}

void AEnemy::MoveToTarget( AActor* target)
{
	if (currentEnemyState == EEnemyState::EES_Dead) return;

	if (enemyController) {
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(target);
		moveRequest.SetAcceptanceRadius(acceptanceRadius);
		moveRequest.SetReachTestIncludesAgentRadius(false);
		FNavPathSharedPtr navPath;
		enemyController->MoveTo(moveRequest, &navPath);
		/*TArray<FNavPathPoint> pathPoints = navPath->GetPathPoints();
		
		if (navPath->GetPathPoints().Num() > 0) {
			for (auto point : patroltargetPoints) {
				DrawDebugSphere(GetWorld(), point->GetActorLocation(), 10.f, 20.F, FColor::Green, true);
			}
		}*/

	}
}

void AEnemy::PatrolCheck()
{
	if (patrolTarget && currentEnemyState== EEnemyState::EES_Patrolling) {

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

				GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemy::PatrolTimerFinished, FMath::RandRange(3, 6));
			}

		}
	}
}

void AEnemy::CombatCheck()
{
	if (combatTarget) {

		if (IsOutofChaseRadius()) {
			ClearTimer(AttackTimer);
			LoseInterestTowardsPlayer();
			StartPatrol();
			UE_LOG(LogTemp, Warning, TEXT("Lost Interest"));
		}
		else if (IsOutOfAttackRadius() && !IsChasing()) {
	
			ClearTimer(AttackTimer);
			StartChasing();
			UE_LOG(LogTemp, Warning, TEXT("Chase Player, Not in Attack Radius"));
		}
		else if (IsInsideAttackRadius() && !IsAttacking() &&!IsEngaged() ) {
			StartAttackTimer();
			UE_LOG(LogTemp, Warning, TEXT("Attacking"));

		}
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(patrolTarget);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& hitImpactPoint)
{
	HandleHealthBar(true);
	DrawDebugSphere( GetWorld(), hitImpactPoint, 10.f, 16, FColor::Black, false, 3.f);
	UE_LOG(LogTemp, Warning, TEXT("GetHitFunction "));
	
	PlayHitVFX(hitImpactPoint);
	PlayHitSound(hitImpactPoint);
	DebugHitPositions(hitImpactPoint);
}

void AEnemy::DebugHitPositions(const FVector& hitImpactPoint)
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

	if (attributeComp->IsAlive()) PlayHitReaction(degrees);
	else OnDeath();

	

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct.GetSafeNormal() * 70, 7, FLinearColor::Green, 5, 2);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Black, FString::Printf(TEXT("Degrees: %f"), degrees));
	}


	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 70, 7, FLinearColor::Yellow, 5, 2);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHitPoint * 70, 7, FLinearColor::Red, 5, 2);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (currentEnemyState == EEnemyState::EES_Dead)  return DamageAmount;

	if (attributeComp && HealthComponet && currentEnemyState!=EEnemyState::EES_Dead ) {
		attributeComp->Receivedamage(DamageAmount);
		HealthComponet->SetHealthPercent(attributeComp->GetHealthPercent());
		combatTarget = EventInstigator->GetPawn();
		currentEnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(combatTarget);
	}
	
	return DamageAmount;
}

void AEnemy::OnPawnSeen(APawn* seenPawn)
{

	bool shouldChase = currentEnemyState != EEnemyState::EES_Dead
		&& seenPawn->ActorHasTag("EchoCharacter")
		&& currentEnemyState != EEnemyState::EES_Attacking;
	
	if (!shouldChase) return;

		UE_LOG(LogTemp, Warning, TEXT("Saw Echo!, Start Chasing"));
		ClearTimer(patrolTimer);
		combatTarget = seenPawn;
		StartChasing();
			
}

void AEnemy::Attack()
{
	Super::Attack();
	currentEnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();	
	
}



void AEnemy::StartPatrol()
{
	currentEnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 125.f;
	MoveToTarget(patrolTarget);

}

void AEnemy::HandleHealthBar(bool bShoulbBeVisible)
{
	if (HealthComponet) HealthComponet->SetVisibility(bShoulbBeVisible);
}

void AEnemy::LoseInterestTowardsPlayer()
{
	combatTarget = nullptr;
	HandleHealthBar(false);

}

bool AEnemy::IsOutofChaseRadius()
{
	return !IstargetInRadius(combatTarget, chaseRadius);
}

void AEnemy::StartChasing()
{
	currentEnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(combatTarget);
}

bool AEnemy::IsOutOfAttackRadius()
{
	return !IstargetInRadius(combatTarget, attackRadius);
}

bool AEnemy::IsChasing()
{
	return currentEnemyState==EEnemyState::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
	return IstargetInRadius(combatTarget, attackRadius);
}

bool AEnemy::IsAttacking()
{
	return currentEnemyState==EEnemyState::EES_Attacking;
}

void AEnemy::StartAttackTimer()
{
	currentEnemyState = EEnemyState::EES_Attacking;
	GetWorldTimerManager().SetTimer(AttackTimer,this, &AEnemy::Attack, FMath::RandRange(1.f, 1.5f));
}

void AEnemy::ClearTimer(FTimerHandle timer)
{
	GetWorldTimerManager().ClearTimer(timer);
}

bool AEnemy::IsDead()
{
	return currentEnemyState == EEnemyState::EES_Dead;
}


void AEnemy::OnDeath()
{
	Super::OnDeath();
	currentEnemyState = EEnemyState::EES_Dead;
	ClearTimer(AttackTimer);
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (HealthComponet) HealthComponet->SetVisibility(false);
	PlayDeathMontage();	

}

bool AEnemy::IstargetInRadius(AActor* targetActor, double radius)
{
	
	double distance = (targetActor->GetActorLocation() - GetActorLocation()).Size();
	DrawDebugSphere(GetWorld(), targetActor->GetActorLocation(), 30.f, 20.F, FColor::Blue);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 30.f, 20.F, FColor::Blue);
	return distance <= radius;
	
}

