// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "Title.generated.h"

class UMenuButton;

UCLASS()
class ELDENRING_API UTitle : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowTitle();
	const bool IsPlayingAnimation() const;

	UFUNCTION()
	FORCEINLINE void SetActivation(const bool IsActivated) { if (!m_is_activated) m_is_activated = IsActivated; }

	UFUNCTION()
	FORCEINLINE bool IsActivated() const { return m_is_activated; }

protected:
	virtual void NativeOnInitialized() override final;

private:
	UPROPERTY(BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMenuButton> NewGame;

	UPROPERTY(BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMenuButton> LoadGame;

	UPROPERTY(BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMenuButton> Exit;

	UPROPERTY(BlueprintReadWrite, Category = "Animation", meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> ShowStartTitle;

	UPROPERTY()
	TWeakObjectPtr<class UUMGSequencePlayer> m_sequence;

	bool m_is_activated;
};
