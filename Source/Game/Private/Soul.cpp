// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"
#include "PickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	StartLineTrace();
	
}

void ASoul::StartLineTrace()
{
	FHitResult hitresult;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(GetOwner());
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		GetActorLocation(),
		GetActorLocation() - FVector(0.f, 0.f, 2000.f),
		objectTypes,
		false,
		ignoreActor,
		EDrawDebugTrace::ForDuration,
		hitresult,
		true
	);
	soulMinHeight += hitresult.ImpactPoint.Z+100.f;
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* pickupInterface = Cast<IPickupInterface>(OtherActor);
	if (pickupInterface) {

		pickupInterface->AddSoul(this);
		SpawnPickupVFX();
		PlayPickupSound();
		Destroy();
	}
	
}

void ASoul::Tick(float DeltaTime)
{
	float currentZlocation = GetActorLocation().Z;

	if (currentZlocation > soulMinHeight)
	{
		FVector deltaLOcation = FVector(0.f, 0.f, driftRate * DeltaTime);
		AddActorLocalOffset(deltaLOcation);
	}
}
