// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Tutorial/TutorialTypes.h"
#include "TutorialHUD.h"
#include "Input/InputActionType.h"
#include "TutorialPersistent.generated.h"

UCLASS()
class ELDENRING_API UTutorialPersistent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	const bool IsActivated() const;

	void Show(const TArray<FTutorialPersistentData>& Slides);
	void Hide();

protected:
	virtual void NativeConstruct() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTutorialHUD> UIP_SlideTop;

	UPROPERTY(EditDefaultsOnly, Category = "HUD", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTutorialHUD> UIP_Slide_Mid;

	UPROPERTY(EditDefaultsOnly, Category = "HUD", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTutorialHUD> UIP_Slide_Bot;

	UPROPERTY()
	TArray<FTutorialPersistentData> m_slides;

	FTimerHandle m_timer;
};
