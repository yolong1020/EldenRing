#pragma once

UENUM(BlueprintType)
enum class EERCameraDirect : uint8
{
	ECD_Focus	UMETA(DisplayName = "Focus"),
	ECD_Front	UMETA(DisplayName = "Front"),
	ECD_Back	UMETA(DisplayName = "Back")
};