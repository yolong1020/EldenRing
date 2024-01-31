// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "YouDied.generated.h"

UCLASS()
class ELDENRING_API UYouDied : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartDirecting(const FAfterPlayerDeathFunc& Callback, const float& DelayTime);

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void EndDirecting();

	UFUNCTION()
	void FilledBlackOut();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> ShowHideNotify;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_delay_time;

	UPROPERTY()
	TWeakObjectPtr<class UObserverManager> m_observe_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TObjectPtr<class UBlackOut> m_black_out;

	FAfterPlayerDeathFunc m_directing_callback;
};
