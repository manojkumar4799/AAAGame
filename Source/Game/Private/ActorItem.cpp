// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorItem.h"
#include "DrawDebugHelpers.h"


#define me 45

#define Draw_Sphere(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),GetActorLocation(), 45.f, 16, FColor::Orange ,false,-1.f );
#define Draw_Vector(startLocation, endLocation) if(GetWorld()){\
DrawDebugLine(GetWorld(),startLocation,endLocation,FColor::Blue,false,-1.f,0,3.f);\
DrawDebugPoint(GetWorld(),endLocation,15.f, FColor::Red,false,-1.f);\
};
// Sets default values
AActorItem::AActorItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

}

// Called when the game starts or when spawned
void AActorItem::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {

		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString("Message"));

    }

	UWorld* world = GetWorld();
	//int bittu = 0;
	//Draw_Sphere();
	
}

// Called every frame
void AActorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float movement = 50;
	float spinRate = 50;
	sinValue += DeltaTime;
	float deltaZ = amptitueOfSin * FMath::Sin(sinValue * waveSpeed); 

	AddActorWorldOffset(FVector(movement * DeltaTime, 0.f, deltaZ));
	AddActorWorldRotation(FRotator(0, spinRate * DeltaTime, 0));
	Draw_Sphere(GetActorLocation())
	Draw_Vector(GetActorLocation(),GetActorLocation()+ GetActorForwardVector()*200)
		
	

}

