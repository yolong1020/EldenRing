#pragma once

//	Common ------------------------------------------------------
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None			UMETA(DisplayName = "None"),
	EWT_StraightSword	UMETA(DisplayName = "Straight Sword"),
	EWT_CrossBow		UMETA(DisplayName = "CrossBow"),
	EWT_Shield			UMETA(DisplayName = "Shield"),
	EWT_Halberd			UMETA(DisplayName = "Halberd"),
	EWT_Body			UMETA(DisplayName = "Body"),
};

UENUM(BlueprintType)
enum class EAttackWeight : uint8
{
	EAW_None		UMETA(DisplayName = "None"),
	EAW_Small		UMETA(DisplayName = "Small Weight Attack"),
	EAW_Medium		UMETA(DisplayName = "Medium Weight Attack"),
	EAW_Heavy		UMETA(DisplayName = "Heavy Weight Attack"),
	EAW_ExtraHeavy	UMETA(DisplayName = "Extra Heavy Weight Attack"),
	EAW_UltraHeavy	UMETA(DisplayName = "Ultra Heavy Weight Attack")
};

UENUM(BlueprintType)
enum class EPhysicalAttackType : uint8
{
	EPAT_Standard	UMETA(DisplayName = "Standard"),
	EPAT_Strike		UMETA(DisplayName = "Strike"),
	EPAT_Slash		UMETA(DisplayName = "Slash"),
	EPAT_Pierce		UMETA(DisplayName = "Pierce")
};