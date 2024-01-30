// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/InputActionType.h"
#include "UI/Tutorial/TutorialData.h"
#include "TutorialHUD.generated.h"

class UOverlay;
class UIconDataAsset;
class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class ELDENRING_API UTutorialHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const bool IsActived() const { return m_is_actived; }
	FORCEINLINE const FText GetText() const;

	void UpdateButton(const FName& Name, const EInputActionType& Type, const bool IsLargeKey);
	void ShowTutorial(const float& Duration = 0);

	UFUNCTION()
	void HideTutorial();
	void HideUI();

	UFUNCTION(BlueprintCallable)
	void SwapVisibility(const bool& On);

protected:
	virtual void NativePreConstruct() override final;
	virtual void NativeConstruct() override final;
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void OpenedTutorial();

	UFUNCTION()
	void ClosedTutorial();
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ShowInteraction;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* HideInteraction;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* Head;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* DiamondBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* TXT;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Icon_Large;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* PressBar;

	UPROPERTY(EditDefaultsOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	UIconDataAsset* IconData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	bool m_has_diamond;

	float m_duration;

	bool m_is_actived;
	FTimerHandle m_timer;
};
