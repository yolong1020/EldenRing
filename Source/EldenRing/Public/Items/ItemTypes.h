#pragma once

//	Potion ------------------------------------------------------

UENUM(BlueprintType)
enum class EPotionSizeType : uint8
{
	EPST_Empty	UMETA(DisplayName = "Empty"),
	EPST_Less	UMETA(DisplayName = "Less"),
	EPST_Half	UMETA(DisplayName = "Half"),
	EPST_Full	UMETA(DisplayName = "Full")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Equipment	UMETA(DisplayName = "Equipment"),
	EIT_Weapon		UMETA(DisplayName = "Weapon"),
	EIT_Consumable	UMETA(DisplayName = "Consumable"),
	EIT_Max			UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EItemOptionType : uint8
{
	EIOT_Attack		UMETA(DisplayName = "Attack"),
	EIOT_Shield		UMETA(DisplayName = "Shield"),
	EIOT_Hp			UMETA(DisplayName = "HP"),
	EIOT_Stamina	UMETA(DisplayName = "Stamina"),
	EIOT_Power		UMETA(DisplayName = "Power"),
	EIOT_Max		UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EItemTradeType : uint8
{
	EITT_Sell		UMETA(DisplayName = "Sell"),
	EITT_Purchase	UMETA(DisplayName = "Purchase")
};