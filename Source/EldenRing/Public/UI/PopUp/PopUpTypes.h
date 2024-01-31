#pragma once

UENUM(BlueprintType)
enum class EPopupType : uint8
{
	EPT_Purchase		UMETA(DisplayName = "Purchase Popup"),
	EPT_Sell			UMETA(DisplayName = "Sell Popup"),
	EPT_NotEnoughGold	UMETA(DisplayName = "Not Enough Gold"),
	EPT_NotEnoughSpace	UMETA(DisplayName = "Not Enough Space"),
	EPT_ExitGame		UMETA(DisplayName = "Exit Game"),
};

UENUM(BlueprintType)
enum class EInteractPopupType : uint8
{
	EIPT_ToNPC			UMETA(DisplayName = "Interact with NPC"),
	EIPT_ToBox			UMETA(DisplayName = "Interact with Box"),
	EIPT_ToBonefire		UMETA(DisplayName = "Interact with Bonefire"),
	EIPT_ToBossArea		UMETA(DisplayName = "Interact with Boss Gate"),
	EIPT_Max			UMETA(DisplayName = "End"),
};