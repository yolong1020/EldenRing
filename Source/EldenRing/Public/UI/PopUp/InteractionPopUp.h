// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/PopUp/PopUpTypes.h"
#include "InteractionPopUp.generated.h"

UCLASS()
class ELDENRING_API UInteractionPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowPopUp(const TObjectPtr<APlayerController>& Controller, const EInteractPopupType& Type, const bool& IsFirst = true);
	void HidePopUp();
	
	FORCEINLINE const EInteractPopupType& GetPopUpType() { return m_type; }

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void OnInteract();

	UFUNCTION()
	void EnableInput();

	UFUNCTION()
	void ClosePopUp();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "PopUp Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "PopUp Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> Desc;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> StartShowPopUp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> StartHidePopUp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIconDataAsset> IconData;

	UPROPERTY()
	TObjectPtr<class ULostGraceDiscovered> m_discovered;

	UPROPERTY()
	TObjectPtr<class UBoneFireMenu> m_bonefire_menu;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> m_controller;

	EInteractPopupType m_type;
	bool m_is_enable_input;
};
