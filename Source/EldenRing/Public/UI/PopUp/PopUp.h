// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Button/ButtonType.h"
#include "UI/UIDelegates.h"
#include "PopUpTypes.h"
#include "PopUp.generated.h"

class UCommonButton;
class UTextBlock;
class URichTextBlock;

UCLASS()
class ELDENRING_API UPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdataTwoButtonForTrade(const EPopupType& PopupType, const TObjectPtr<class UItemObject> ItemObject, const FPopupActiveBtn& OnActive, const FPopupCancelBtn& OnCancel);
	void UpdateTwoButtonForExitGame(const EPopupType& PopupType, const FPopupActiveBtn& OnActive, const FPopupCancelBtn& OnCancel);
	void UpdateOneButtonLayout(const EPopupType& PopupType, const FPopupConfirmlBtn& OnConfirm);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTwoBtnTextForTrade(const FString& ItemName, const FString& ItemPrice, const EPopupType& PopupType);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTwoBtnText(const EPopupType& PopupType);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOneBtnText(const EPopupType& PopupType);
	
protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void OnActive();

	UFUNCTION()
	void OnCancel();

	UFUNCTION()
	void OnConfirm();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	UCommonButton* Active;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	UCommonButton* Cancel;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	UCommonButton* Confirm;

	UPROPERTY(BlueprintReadWrite, Category = "PopUp Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	UTextBlock* Title;

	UPROPERTY(BlueprintReadWrite, Category = "PopUp Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	URichTextBlock* RichTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopUp Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "ture"))
	EPopupButtonType m_button_type;

	FPopupActiveBtn		m_on_active;
	FPopupCancelBtn		m_on_cancel;
	FPopupConfirmlBtn	m_on_confirm;
};
