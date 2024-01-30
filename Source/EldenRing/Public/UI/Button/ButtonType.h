#pragma once

UENUM(BlueprintType)
enum class EMenuButtonType : uint8
{
	EMBT_NewGame	UMETA(DisplayName = "NewGame"),
	EMBT_LoadGame	UMETA(DisplayName = "LoadGame"),
	EMBT_Exit		UMETA(DisplayName = "Exit")
};

UENUM(BlueprintType)
enum class EPopupButtonType : uint8
{
	EPBT_OneBtn		UMETA(DisplayName = "One Button"),
	EPBT_TwoBtn		UMETA(DisplayName = "Two Button")
};

UENUM(BlueprintType)
enum class ESelectPopupButtonType : uint8
{
	ESPBT_ToCommunicate		UMETA(DisplayName = "Communicate with NPC"),
	ESPBT_ToTrade			UMETA(DisplayName = "Trade with NPC"),
	ESPBT_Max				UMETA(DisplayName = "End"),
};