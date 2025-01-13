// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "SelectionPopUp.generated.h"

UCLASS()
class ELDENRING_API USelectionPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowPopUp(const TObjectPtr<APlayerController>& Controller);
	void SetOptionButtons(const uint8& FirstType, const uint8& SecondType);

protected:
	virtual void NativeOnInitialized() override final;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override final;

private:
	void FindButtonFunction(class USelectionButton* const button, const uint8& Type);

	UFUNCTION()
	void ClosePopUp();

	UFUNCTION()
	void CloseAndCommunicate();

	UFUNCTION()
	void CloseAndTrade();

	UFUNCTION()
	void OnConfirm();

	UFUNCTION()
	void OnEscape();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class USelectionButton* FirstOption;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class USelectionButton* SecondOption;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* StartShowSelection;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* StartHideSelection;

	UPROPERTY()
	class UTradeManager* m_trade_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	class APlayerController* m_controller;
};
