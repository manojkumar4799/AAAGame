// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorItem.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AWeapon : public AActorItem
{
	GENERATED_BODY()

protected:
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override ;
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	void Equip(USceneComponent* inParent, FName socketname);

	void AttachMeshToSocket(USceneComponent* inParent, const FName& socketname);

private:

	UPROPERTY(EditAnywhere)
	USoundBase* bladeSound;
	
};
