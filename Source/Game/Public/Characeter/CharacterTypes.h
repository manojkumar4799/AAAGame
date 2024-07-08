#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Equiped UMETA(DisplayName = "Equiped"),
	ECS_Unequiped UMETA(DisplayName = "Unequiped")
};

UENUM(BlueprintType)
enum class EActionState :uint8 {
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum class EDeathStatus : uint8 {
	EDS_Alive UMETA(Displayname="alive"),
	EDS_DeathPose1 UMETA(Displayname = "DeathPose1"),
	EDS_DeathPose2 UMETA(Displayname = "DeathPose2"),
	EDS_DeathPose3 UMETA(Displayname = "DeathPose3"),
	EDS_DeathPose4 UMETA(Displayname = "DeathPose4")
};