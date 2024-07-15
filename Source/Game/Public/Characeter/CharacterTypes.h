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
	EDS_Alive UMETA(DisplayName ="alive"),
	EDS_DeathPose1 UMETA(DisplayName = "DeathPose1"),
	EDS_DeathPose2 UMETA(DisplayName = "DeathPose2"),
	EDS_DeathPose3 UMETA(DisplayName = "DeathPose3"),
	EDS_DeathPose4 UMETA(DisplayName = "DeathPose4")
};

UENUM(BlueprintType)
enum class EEnemyState :uint8 {
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Dead UMETA(DisplayName = "Dead")
};