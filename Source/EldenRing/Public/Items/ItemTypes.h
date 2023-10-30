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