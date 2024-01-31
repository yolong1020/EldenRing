#pragma once

UENUM(BlueprintType)
enum class EAssemblyPointObjectType : uint8
{
	EAPOT_None			UMETA(DisplayName = "None"),
	EAPOT_Bench			UMETA(DisplayName = "Bench"),
	EAPOT_Floor			UMETA(DisplayName = "Floor"),
	EAPOT_Max			UMETA(DisplayName = "Max")
};