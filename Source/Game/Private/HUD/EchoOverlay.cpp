// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEchoOverlay::SetProgressBar(UProgressBar* progressBar, float percent)
{
	if (progressBar) {

		progressBar->SetPercent(percent);
	}
}

void UEchoOverlay::SetResourcesCount(UTextBlock* textBlock, int32 count)
{
	if (textBlock)
	{
		textBlock->SetText(FText::FromString(FString::Printf(TEXT("% d"), count)));
	}
}
