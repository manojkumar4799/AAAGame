// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEchoOverlay::SetHealthBar(float percent)
{
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(percent);
	}
}

void UEchoOverlay::SetStaminaBar(float percent)
{
	if (StaminaProgressBar) {
		StaminaProgressBar->SetPercent(percent);
	}
}

void UEchoOverlay::SetGoldCount(int32 count)
{
	if (GoldText) {
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), count)));
	}
}

void UEchoOverlay::SetSoulCount(int32 count)
{
	if (SoulText) {
		SoulText->SetText(FText::FromString(FString::Printf(TEXT("%d"), count)));
	}
}
