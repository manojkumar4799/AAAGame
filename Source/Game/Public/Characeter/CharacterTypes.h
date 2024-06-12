#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Equiped UMETA(DisplayName = "Equiped"),
	ECS_Unequiped UMETA(DisplayName = "Unequiped")
};

UENUM(BlueprintType)
enum class EActionState :uint8 {
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};