// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorItem.generated.h"


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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float sinValue;
	float amptitueOfSin = 1;
	float waveSpeed=10;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* staticMesh;



};
