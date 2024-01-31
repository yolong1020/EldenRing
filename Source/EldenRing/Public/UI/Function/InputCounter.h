// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCounter.generated.h"

class AC0000;
class UButton;
class UEditableText;

UCLASS()
class ELDENRING_API UInputCounter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void  InitInputCounter();
	int32 GetTextAmount();
	
private:
	UFUNCTION()
	void OnClickUpper();

	UFUNCTION()
	void OnClickUnder();
	
	UFUNCTION()
	void OnChnageTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	void SetTextAsCurrency(const int32& amount);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ButtonUpper;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ButtonUnder;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UEditableText* InputText;

	AC0000* m_owner;
};
