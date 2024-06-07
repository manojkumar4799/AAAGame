// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "ActorItem.h"
#include "Characeter/GameCharacter.h"



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
	RootComponent = staticMesh;
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AActorItem::BeginPlay()
{	
	Super::BeginPlay();
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AActorItem::OnSphereOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AActorItem::OnSphereOverlapEnd);
	if (GEngine) {

		//GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Yellow, FString("Message"));

    }

	UWorld* world = GetWorld();
	//int bittu = 0;
	//Draw_Sphere();
	
}

float AActorItem::TransformSin()
{
	return amptitueOfSin * FMath::Sin(sinValue * waveSpeed);
}

// Called every frame
void AActorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float spinRate = 100;
	sinValue += DeltaTime;
	//float deltaZ = amptitueOfSin * FMath::Sin(sinValue * waveSpeed); 

	//AddActorWorldOffset(FVector(0.f, 0.f, deltaZ));
	//AddActorWorldRotation(FRotator(0, spinRate * DeltaTime, 0));
	//Draw_Sphere(GetActorLocation())
	//Draw_Vector(GetActorLocation(),GetActorLocation()+ GetActorForwardVector()*200)
		
	

}

void AActorItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter* echoCharacter = Cast<AGameCharacter>(OtherActor);
	if (echoCharacter) {

		echoCharacter->SetOverlapingItem(this);

	}

}

void AActorItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* echoCharacter = Cast<AGameCharacter>(OtherActor);
	if (echoCharacter) {

		echoCharacter->SetOverlapingItem(nullptr);

	}

}

