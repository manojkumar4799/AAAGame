// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HitInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"



class UGeometryCollectionComponent;
UCLASS()
class GAME_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	void GetHit_Implementation(const FVector& hitImpactPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* geometryObject;
	

};
