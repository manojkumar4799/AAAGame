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
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector startpoint = boxTraceStartPoint->GetComponentLocation();
	const FVector endPoint = boxTraceEndPoint->GetComponentLocation();
	TArray<AActor*> ignoreList;
	if (ignoreActor) {
		ignoreList.Add(ignoreActor);
	}
	FHitResult hitResult;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		startpoint,
		endPoint,
		FVector(2.f, 2.f, 2.f),
		boxTraceStartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoreList,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	if (hitResult.GetActor()) {

		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());

		if (hitInterface) {
			hitInterface->Execute_GetHit(hitResult.GetActor(), hitResult.ImpactPoint);
		}
		ignoreActor = hitResult.GetActor();
		CreateFieldForce(hitResult.ImpactPoint);
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}

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


