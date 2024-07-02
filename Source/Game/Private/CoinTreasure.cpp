// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinTreasure.h"
#include "Characeter/GameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACoinTreasure::ACoinTreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoinTreasure::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoinTreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter* echoChar = Cast<AGameCharacter>(OtherActor);
	if(echoChar) {
		UGameplayStatics::PlaySoundAtLocation(this, pickupSound, GetActorLocation());
		Destroy();
	}
}

// Called every frame
void ACoinTreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

