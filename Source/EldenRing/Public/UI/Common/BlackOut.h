// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "BlackOut.generated.h"

UCLASS()
class ELDENRING_API UBlackOut : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowBlackOutNoAnimation();
	void ShowBlackOut();
	void ShowBlackOut(const FAfterShowBlackOutFunc& Callback);
	void HideBlackOut(const FAfterHideBlackOutFunc& Callback);
	const bool IsPlayingAnimation() const;

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void EndShowBlackOut();

	UFUNCTION()
	void EndHideBlackOut();
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> ShowStartBlackOut;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> HideStartBlackOut;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBorder> BlackBorder;

	UPROPERTY()
	TWeakObjectPtr<class UUMGSequencePlayer> m_sequence;

	FAfterShowBlackOutFunc m_show_callback;
	FAfterHideBlackOutFunc m_hide_callback;
};
