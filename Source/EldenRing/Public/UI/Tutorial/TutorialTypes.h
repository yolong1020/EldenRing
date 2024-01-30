#pragma once

UENUM(BlueprintType)
enum class ETutorialPosition : uint8
{
	ETP_Top	UMETA(DisplayName = "Top"),
	ETP_Mid	UMETA(DisplayName = "Mid"),
	ETP_Bot	UMETA(DisplayName = "Bot"),
	ETP_Max	UMETA(DisplayName = "Max")
};