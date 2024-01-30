// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "ButtonType.h"
#include "MenuButton.generated.h"

UCLASS()
class ELDENRING_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

	FORCEINLINE void InitCallback(const FActivateMenuButtonFunc& Activated, const FCheckActivateMenuButtonFunc& CheckActivate, const FCHeckAnimMenuButtonFunc& CheckAnim) 
	{ 
		m_activated = Activated;  m_check_activate = CheckActivate; m_check_directing = CheckAnim; 
	}

protected:
	virtual void NativeOnInitialized() override final;
	virtual void NativePreConstruct() override final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;

private:
	UFUNCTION()
	void ButtonFunc();

	UFUNCTION()
	void OpenNextLevel();

private:
	UPROPERTY(BlueprintReadWrite, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> TextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> Button;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> Hover;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> HoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> ClickSound;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (AllowPrivateAccess = "true"))
	FText TextContext;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (AllowPrivateAccess = "true"))
	EMenuButtonType ButtonType;

	FActivateMenuButtonFunc			m_activated;
	FCheckActivateMenuButtonFunc	m_check_activate;
	FCHeckAnimMenuButtonFunc		m_check_directing;
	FTimerHandle m_timer;
};
