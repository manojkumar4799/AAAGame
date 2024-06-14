// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorItem.generated.h"

class USphereComponent;

enum class EItemState :uint8 {

	EIS_Hovering,
	EIS_Equipped
};
UCLASS()

class GAME_API AActorItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* staticMesh;

	

	UFUNCTION(BlueprintPure)
	float TransformSin();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EItemState itemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* sphere;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	

private:
	UPROPERTY(EditAnywhere)
	float sinValue;
	UPROPERTY(EditAnywhere)
	float amptitueOfSin = 1;
	UPROPERTY(EditAnywhere)
	float waveSpeed=10;

	

	

	
	




};
