// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommunicateTalk.generated.h"

UCLASS()
class ELDENRING_API UCommunicateTalk : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const bool IsPlayAnimation() { return m_is_playing; }

	UFUNCTION()
	FORCEINLINE void FinishAnimation() { m_is_playing = false; }

	void InitCallOut(class UCommunicateCallOut* const CallOut);
	void StartDialouge(const FText& Dialouge);
	void UpdateDialouge(const FText& Dialouge);
	void HideDialouge();

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void EndDialouge();

	UFUNCTION()
	void ShowTalkScript();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess))
	class UTextBlock* TalkScript;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ShowCommunicate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* HideCommunicate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ShowDialogue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* HideDialogue;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	class UCommunicateCallOut* m_callout;

	FText m_text;
	bool m_is_playing = false;
};
