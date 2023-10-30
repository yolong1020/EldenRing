#pragma once

//	Quick Slot ------------------------------------------------------
UENUM(BlueprintType)
enum class EQuickSlotType : uint8
{
	EQST_Top		UMETA(DisplayName = "Slot Top"),
	EQST_Bottom		UMETA(DisplayName = "Slot Bottom"),
	EQST_Left		UMETA(DisplayName = "Slot Left"),
	EQST_Right		UMETA(DisplayName = "Slot Right")
};