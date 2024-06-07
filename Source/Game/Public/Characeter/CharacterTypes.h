#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Equiped UMETA(DisplayName = "Equiped"),
	ECS_Unequiped UMETA(DisplayName = "Unequiped")
};