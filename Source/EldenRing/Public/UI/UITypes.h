#pragma once

//	Drag and Drop HUD ------------------------------------------------------

UENUM(BlueprintType)
enum class EDragAndDropHUD : uint8
{
	EDAD_None			UMETA(DisplayName = "None"),
	EDAD_Inventory		UMETA(DisplayName = "Inventory"),
	EDAD_Rooting		UMETA(DisplayName = "Rooting"),
	EDAD_Equipment		UMETA(DisplayName = "Equipment"),
	EDAD_Vender			UMETA(DisplayName = "Vender"),
	EDAD_Max			UMETA(DisplayName = "Max")
};