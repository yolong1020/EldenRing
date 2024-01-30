#pragma once

#include "UI/Button/ButtonType.h"
#include "CommunicateScriptData.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FCommunicateScriptData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FCommunicateScriptData() :
		DialogueText(""), IsTriggerEvent(false) {}
	~FCommunicateScriptData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Options;

	// Max 2 Buttons.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> ButtonType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EventFunctions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTriggerEvent;

};