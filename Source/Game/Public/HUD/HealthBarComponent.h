// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	void SetHealthPercent(float percent);

private:
	class UHealthBar* healthBarWidget;
};
