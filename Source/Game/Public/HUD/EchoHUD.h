// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EchoHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AEchoHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;


private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEchoOverlay> echoOverlayClass;

	UPROPERTY()
	class UEchoOverlay* echoOverlay;

public:

	FORCEINLINE class UEchoOverlay* GetEchoOverlay() { return echoOverlay; }
	
};
