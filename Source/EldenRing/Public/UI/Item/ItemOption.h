// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemTypes.h"
#include "ItemOption.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ELDENRING_API UItemOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitOptionText(const EItemOptionType& OptionType, const FText& OptionValue);

private:
	UPROPERTY(BlueprintReadWrite, Category = "Option Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* Option;

	UPROPERTY(EditAnywhere, Category = "Option Text", meta = (editcondition = "bOverride_Text"))
	FText OptionText;

	UPROPERTY(BlueprintReadWrite, Category = "Value Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* Value;

	UPROPERTY(EditAnywhere, Category = "Value Text", meta = (editcondition = "bOverride_Text"))
	FText ValueText;
};
