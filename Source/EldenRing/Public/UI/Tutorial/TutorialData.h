#pragma once

#include "Input/InputActionType.h"
#include "UI/Tutorial/TutorialTypes.h"
#include "TutorialData.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FTutorialPersistentData
{
	GENERATED_BODY()

	explicit FTutorialPersistentData() : 
		Name(""), Action(), ShowPosition(ETutorialPosition::ETP_Max), IsLargeKey(false) {}
	explicit FTutorialPersistentData(const FName& Name, const EInputActionType& Type, const ETutorialPosition& Position, const bool IsLarge) :
		Name(Name), Action(Type), ShowPosition(Position), IsLargeKey(IsLarge) {}
	~FTutorialPersistentData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputActionType Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialPosition ShowPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLargeKey;
};