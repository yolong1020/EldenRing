#pragma once

#include "InputActionType.h"
#include "InputIconData.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FIconData
{
	GENERATED_BODY()

	explicit FIconData() :
		Text(""), Action(EInputActionType::EIAT_Attack), MaterialInstance(nullptr) {}
	~FIconData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputActionType Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* MaterialInstance;
};