// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupInterface.h"

// Add default functionality here for any IPickupInterface functions that are not pure virtual.

void IPickupInterface::SetOverlappingItem(AActorItem* item)
{

}

void IPickupInterface::AddSoul(ASoul* soul)
{
	UE_LOG(LogTemp, Warning, TEXT(" IPickupInterface::AddSoul"));
}

void IPickupInterface::AddGold(ACoinTreasure* treasure)
{
	UE_LOG(LogTemp, Warning, TEXT("IPickupInterface::AddGold"));

}
