// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorItem.h"
#include "Weapon.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS()
class GAME_API AWeapon : public AActorItem
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override ;
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* boxTraceStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* boxTraceEndPoint;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFieldForce(const FVector& fieldForceLocation);

	

public:
	AWeapon();
	void Equip(USceneComponent* inParent, FName socketname, AActor* newOwner, APawn* Instigator);

	void AttachMeshToSocket(USceneComponent* inParent, const FName& socketname);

	UBoxComponent* GetBoxCollider();

	AActor* ignoreActor;
private:

	UPROPERTY(EditAnywhere)
	USoundBase* bladeSound;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollider;

	UPROPERTY(EditAnywhere)
	float damage = 20;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
