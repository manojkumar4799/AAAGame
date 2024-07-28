// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActors/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "CoinTreasure.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	outerColliderCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColliderCapsule"));
	SetRootComponent(outerColliderCapsule);
	outerColliderCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	outerColliderCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	geometryObject = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomteryObjectRef"));
	geometryObject->SetupAttachment(GetRootComponent());
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
		SetLifeSpan(3);
		outerColliderCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& hitImpactPoint, AActor* hitter)
{
	
}

