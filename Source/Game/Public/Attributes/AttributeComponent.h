// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
	float health;

	UPROPERTY(EditAnywhere)
	float maxHealth;

	UPROPERTY(EditAnywhere)
	int32 soulCount;

	UPROPERTY(EditAnywhere)
	int32 goldCount;

public:

	void Receivedamage(float damage);
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	FORCEINLINE int32 GetGoldCount() { return goldCount; }

	FORCEINLINE int32 GetSoulCount() { return soulCount; }

	void AddGold(int32 count);

	void AddSoul(int32 count);
	
	bool IsAlive();


		
};
