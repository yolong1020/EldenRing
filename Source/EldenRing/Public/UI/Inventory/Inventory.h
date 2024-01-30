// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

UCLASS()
class ELDENRING_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetController(const TWeakObjectPtr<class APlayerController>& Controller) { m_controller = Controller; }
	
protected:
	virtual void   NativeOnInitialized() override final;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override final;

private:
	UFUNCTION()
	void ExitInventory();

private:
	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;

	UPROPERTY()
	TWeakObjectPtr<class UStorageManager> m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

	FLatentActionInfo m_callback;
};
