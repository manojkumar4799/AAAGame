// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::Receivedamage(float damage)
{
	health = FMath::Clamp(health - damage, 0.f, 100.f);
}

float UAttributeComponent::GetHealthPercent()
{
	return health/maxHealth;
}

void UAttributeComponent::RegenStamina(float deltaTime)
{
	stamina = FMath::Clamp(stamina + staminaIncreaseRate * deltaTime, 0.f, maxStamina);
}

void UAttributeComponent::UseStamina()
{
	stamina = FMath::Clamp(stamina - staminaCost, 0.f, maxStamina);
}

void UAttributeComponent::IncreaseStamina()
{
}

void UAttributeComponent::AddGold(int32 count)
{
	goldCount += count;
}

void UAttributeComponent::AddSoul(int32 count)
{
	soulCount += count;
}

bool UAttributeComponent::IsAlive()
{
	return health>0.f;
}

