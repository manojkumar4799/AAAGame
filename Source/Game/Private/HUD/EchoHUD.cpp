// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoHUD.h"
#include "HUD/EchoOverlay.h"

void AEchoHUD::BeginPlay()
{
	Super::BeginPlay();
	echoOverlay= CreateWidget<UEchoOverlay>(GetWorld()->GetFirstPlayerController(), echoOverlayClass);
	echoOverlay->AddToViewport();
}

