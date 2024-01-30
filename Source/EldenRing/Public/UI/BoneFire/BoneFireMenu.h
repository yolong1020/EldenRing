// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "BoneFireMenu.generated.h"

UCLASS()
class ELDENRING_API UBoneFireMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowBonefireMenu(const TObjectPtr<APlayerController>& Controller);

protected:
	virtual void NativeOnInitialized() override final;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override final;

private:
	UFUNCTION()
	void HideBlackOut();

	UFUNCTION()
	void CloseMenu();

	UFUNCTION()
	void OpenInteractPopUp();

	void ExitMenu();
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess))
	TObjectPtr<class UTextBlock> LocationName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess))
	TObjectPtr<class UBoneFireButton> RecoverBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess))
	TObjectPtr<class UBoneFireButton> LeaveBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	TObjectPtr<UWidgetAnimation> StartEnterGrace;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	TObjectPtr<UWidgetAnimation> StartOpenMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	TObjectPtr<UWidgetAnimation> StartHideMenu;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

	UPROPERTY()
	TObjectPtr<class APlayerController> m_controller;
};
