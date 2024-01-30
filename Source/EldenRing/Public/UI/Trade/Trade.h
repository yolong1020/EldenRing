// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Trade.generated.h"

UCLASS()
class ELDENRING_API UTrade : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetController(const TWeakObjectPtr<class APlayerController>& Controller) { m_controller = Controller; }

protected:
	virtual void   NativeOnInitialized() override final;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override final;

private:
	UFUNCTION()
	void ExitTrade();

private:
	UPROPERTY()
	TWeakObjectPtr<class UTradeManager> m_trade_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;

	FLatentActionInfo m_callback;
};
