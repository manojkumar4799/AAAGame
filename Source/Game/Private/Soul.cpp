// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"
#include "PickupInterface.h"

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
