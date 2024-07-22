// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/Weapon.h"
#include "Characeter/GameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HitInterface.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderComp"));
	boxCollider->SetupAttachment(GetRootComponent());

	boxTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	boxTraceStartPoint->SetupAttachment(GetRootComponent());
	boxTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	boxTraceEndPoint->SetupAttachment(GetRootComponent());

	boxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

//Overlaping section start


void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(FName("Enemy")) && OtherActor->ActorHasTag(FName("Enemy"))) return;
	FHitResult hitResult;
	BoxTrace(hitResult);
	ignoreActor = hitResult.GetActor();

	if (hitResult.GetActor()) {

		if (GetOwner()->ActorHasTag(FName("Enemy")) && hitResult.GetActor()->ActorHasTag(FName("Enemy"))) return;
		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		if (hitInterface) {
			hitInterface->Execute_GetHit(hitResult.GetActor(), hitResult.ImpactPoint);
		}
		
		CreateFieldForce(hitResult.ImpactPoint);
		
	}

}

void AWeapon::BoxTrace(FHitResult& hitResult)
{
	const FVector startpoint = boxTraceStartPoint->GetComponentLocation();
	const FVector endPoint = boxTraceEndPoint->GetComponentLocation();
	TArray<AActor*> ignoreList;
	if (ignoreActor) {
		ignoreList.Add(ignoreActor);
	}
	

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		startpoint,
		endPoint,
		FVector(2.f, 2.f, 2.f),
		boxTraceStartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoreList,
		bShowDebugTraceBox? EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
		hitResult,
		true
	);

}




//overlaping section end

//weapon related code start



void AWeapon::Equip(USceneComponent* inParent, FName socketname, AActor* newOwner, APawn* instigator)
{
	SetOwner(newOwner);
	SetInstigator(instigator);
	AttachMeshToSocket(inParent, socketname);
	itemState = EItemState::EIS_Equipped;
	if (bladeSound) {
		UGameplayStatics::PlaySoundAtLocation(this, bladeSound, GetActorLocation());

	}
	if (sphere) {
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (spawnVFX) {
		spawnVFX->Deactivate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* inParent, const FName& socketname)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	staticMesh->AttachToComponent(inParent, transformRules, socketname);
}
UBoxComponent* AWeapon::GetBoxCollider()
{
	return boxCollider;
}
//weapon related code end


