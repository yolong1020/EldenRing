// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "CommunicateCallOut.generated.h"

UCLASS()
class ELDENRING_API UCommunicateCallOut : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartCommunicate(const TObjectPtr<APlayerController>& Controller);
	void StartOptionCommunicate(const FString& Key);
	void InitThreatenedDialogue();

protected:
	virtual void NativeOnInitialized() override final;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void UpdateCommunicate();

	UFUNCTION()
	void EndCommunicate();

	UFUNCTION()
	void EndOptionCommunicate();

	UFUNCTION()
	void SelectOption();

	UFUNCTION()
	void OpenTrade();

	UFUNCTION()
	void OpenInteractPopUp();

	UFUNCTION()
	void OpenSwapTutorial();

	UFUNCTION()
	void SaveGameData();

private:
	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UTradeManager> m_trade_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

	UPROPERTY(EditDefaultsONLY, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UCommunicateTalk* TalkBox;
	
	UPROPERTY()
	UDataTable* m_dialouge;

	FCommunicateScriptData m_data_cur;
	FCommunicateScriptData m_data_treatened;
	FLatentActionInfo	   m_delay_action;

	bool m_is_threatened;
};
