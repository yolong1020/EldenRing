#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	EIAT_Jump		UMETA(DisplayName = "Jump"),
	EIAT_Dodge		UMETA(DisplayName = "Dodge"),
	EIAT_Attack		UMETA(DisplayName = "Attack"),
	EIAT_Interact	UMETA(DisplayName = "Interact"),
	EIAT_LockOn		UMETA(DisplayName = "LockOn"),
	EIAT_Swap		UMETA(DisplayName = "Swap"),
	EIAT_Sprint		UMETA(DisplayName = "Sprint"),
	EIAT_Walk		UMETA(DisplayName = "Walk"),
	EIAT_Guard		UMETA(DisplayName = "Guard"),
	EIAT_Parry		UMETA(DisplayName = "Parry"),
	EIAT_StrongAtk	UMETA(DisplayName = "StrongAtk"),
	EIAT_Potion		UMETA(DisplayName = "Potion"),
};