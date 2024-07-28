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
    
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFieldForce(const FVector& fieldForceLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* boxTraceStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* boxTraceEndPoint;	

	

public:
	AWeapon();
	void Equip(USceneComponent* inParent, FName socketname, AActor* newOwner, APawn* Instigator);

	void AttachMeshToSocket(USceneComponent* inParent, const FName& socketname);

	UBoxComponent* GetBoxCollider();

	AActor* ignoreActor;
private:


	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void BoxTrace(FHitResult& hitResult);


	UPROPERTY(EditAnywhere)
	FVector boxTraceExtent = FVector(5.f);
	UPROPERTY(EditAnywhere)
	bool bShowDebugTraceBox = false;
	UPROPERTY(EditAnywhere)
	USoundBase* bladeSound;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollider;

	UPROPERTY(EditAnywhere)
	float damage = 20;

	
};
