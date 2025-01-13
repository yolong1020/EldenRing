// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModalPopUp.generated.h"

class UButton;
class UTextBlock;
class UInputCounter;
class UStorageManager;

UCLASS()
class ELDENRING_API UModalPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitModal(const FString& title);

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

private:
	UFUNCTION()
	void OnClickConfirm();

	UFUNCTION()
	void OnClickCancel();

private:
	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* ModalTitle;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ButtonConfirm;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ButtonCancel;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInputCounter* InputCounter;

	UPROPERTY()
	TWeakObjectPtr<class UStorageManager> m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;
};
