// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActors/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "CoinTreasure.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	geometryObject = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomteryObjectRef"));
	SetRootComponent(geometryObject);
	geometryObject->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	geometryObject->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreakChoas);
	
}

void ABreakableActor::OnBreakChoas(const FChaosBreakEvent& BreakEvent)
{
	if (!btreasureSpwaned &&TreasureClasses.Num() > 0)
	{
		int32 treasureTyoeSelection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		GetWorld()->SpawnActor<ACoinTreasure>(TreasureClasses[treasureTyoeSelection], GetActorLocation(), GetActorRotation());
		btreasureSpwaned = true;
	}
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& hitImpactPoint)
{
	
}

