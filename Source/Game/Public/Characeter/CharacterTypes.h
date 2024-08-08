#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Equiped UMETA(DisplayName = "Equiped"),
	ECS_Unequiped UMETA(DisplayName = "Unequiped")
};

UENUM(BlueprintType)
enum class EActionState :uint8 {
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReact UMETA(DisplayName = "HitReact"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_Dead UMETA(DisplayName="Dead")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8 {
	EDP_DeathPose1 UMETA(DisplayName = "DeathPose1"),
	EDP_DeathPose2 UMETA(DisplayName = "DeathPose2"),
	EDP_DeathPose3 UMETA(DisplayName = "DeathPose3"),
	EDP_DeathPose4 UMETA(DisplayName = "DeathPose4")
};

UENUM(BlueprintType)
enum class EEnemyState :uint8 {
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Engaged UMETA(DisplayName="EngagedInCombat"),
	EES_NoState UMETA(DisplayName="NoState")
};