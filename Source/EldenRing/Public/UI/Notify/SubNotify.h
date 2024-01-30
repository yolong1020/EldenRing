// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubNotify.generated.h"

UCLASS()
class ELDENRING_API USubNotify : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowSubNotify(const FText& InText);
	
protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void HideSubNotify();

	UFUNCTION()
	void DelayNextProcess();

	UFUNCTION()
	void CloseSubNotify();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWideget), meta = (AllowPrivateAccess))
	class UTextBlock* Message;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	UWidgetAnimation* StartShowNotify;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	UWidgetAnimation* StartHideNotify;

	UPROPERTY(BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> NotifySound;

	FLatentActionInfo m_delay_callback;
};
