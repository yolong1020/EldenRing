#pragma once

//	Equipment ------------------------------------------------------

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_None			UMETA(DisplayName = "None"),
	EET_Head			UMETA(DisplayName = "Equipment Head"),
	EET_Body			UMETA(DisplayName = "Equipment Body"),
	EET_Arms			UMETA(DisplayName = "Equipment Arms"),
	EET_Legs			UMETA(DisplayName = "Equipment Legs"),
	EET_Weapon			UMETA(DisplayName = "Equipment Weapon"),
	EET_Consumable		UMETA(DisplayName = "Equipment Consumable"),
	EET_Max				UMETA(DisplayName = "Max")
};