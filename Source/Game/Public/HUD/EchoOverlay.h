// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EchoOverlay.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UEchoOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBar( float percent);
	void SetStaminaBar(float percent);

	void SetGoldCount(int32 count);
	void SetSoulCount(int32 count);

private:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta= (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulText;
	
};
