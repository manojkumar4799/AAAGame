// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorItem.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API ASoul : public AActorItem
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;



private:

	void StartLineTrace();
	UPROPERTY(EditAnywhere)
	int32 soulCount;

	UPROPERTY(EditAnywhere)
	float soulMinHeight;
	UPROPERTY(EditAnywhere)
	float driftRate = -20.f;


	
public:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE int32 GetSoulValue() { return soulCount; }
	FORCEINLINE void SetSoulValue(int32 value) { soulCount= value; }

};
