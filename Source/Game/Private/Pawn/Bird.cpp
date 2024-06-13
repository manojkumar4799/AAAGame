// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Pawn/Bird.h"



// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsuleCompnent"));
	SetRootComponent(capsule);

	birdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	birdMesh->SetupAttachment(GetRootComponent());
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	cameraArm->SetupAttachment(GetRootComponent());

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(cameraArm);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABird::MoveForward(float value) 
{
	if (Controller && value != 0) 
	{
		AddMovementInput(GetActorForwardVector(), value);
		UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	}
}

void ABird::Turn(float value)
{
	AddControllerYawInput(value);
	 
}

void ABird::LookUp(float value)
{
	AddControllerPitchInput(value);
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);
}

