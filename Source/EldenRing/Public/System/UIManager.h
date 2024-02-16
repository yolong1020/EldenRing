// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UIDelegates.h"
#include "UI/PopUp/PopUpTypes.h"
#include "UI/Tutorial/TutorialData.h"
#include "UIManager.generated.h"

UCLASS()
class ELDENRING_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	const bool IsPlayingBalckout() const;
	const bool IsBlackOutInViewport() const;

	void ShowBlackOutNoAnimation();
	void ShowBlackOut(const FAfterShowBlackOutFunc& Callback);
	void HideBlackOut(const FAfterHideBlackOutFunc& Callback);
	
	FORCEINLINE const bool IsEmptyUI() const										{ return m_container.IsEmpty(); }
	FORCEINLINE const bool IsTopUI(const TWeakObjectPtr<UUserWidget>& Widget) const { if (m_container.IsEmpty()) return true; return m_container.Last() == Widget; }
	void RegistUI(TWeakObjectPtr<UUserWidget> Widget, const int32& ZOrder = 0);
	void UnregistUI();

	void OpenExitPopUp(const TWeakObjectPtr<APlayerController>& Controller);
	void OpenOneBtnPopUp(const EPopupType& Type, const FPopupConfirmlBtn& Confirm);
	void OpenTwoBtnPopUp(const EPopupType& Type, const FPopupActiveBtn& Active, const FPopupCancelBtn& Cancel);

	void OpenSubNotify(const FText& InText);

	void OpenTutorialGroupCall(const TArray<FTutorialPersistentData>& Slides);
	void CloseTutorialGroupCall();
	void OpenTutorialCallout(const FName& Name, const EInputActionType& Type, const bool IsLargeKey, const float& Duration = 0);
	void CloseTutorialCallout();
	const bool IsActiveCallout(const FName& Name);
	void CloseAllTutorial();

	const bool IsLockOnInViewport() const;
	void ShowLockOn(const TWeakObjectPtr<APlayerController> Controller, const TWeakObjectPtr<class AGameCharacter> Target);
	void HideLockOn();

private:
	void CreateBlackOut();
	void CreatePopUp();
	void CreateTutorialGroupCall();
	void CreateTutorialCallout();

	
	UFUNCTION()
	void ExitPopUpAction();

private:
	UPROPERTY()
	TObjectPtr<class UBlackOut> m_black_out;

	UPROPERTY()
	TObjectPtr<class UPopUp> m_popup;

	UPROPERTY()
	TObjectPtr<class USubNotify> m_sub_notify;

	UPROPERTY()
	TObjectPtr<class UTutorialGroupCall> m_tutorial_group;

	UPROPERTY()
	TArray<TWeakObjectPtr<class UUserWidget>> m_container;

	UPROPERTY()
	TObjectPtr<class UTutorialCallout> m_callout;

	UPROPERTY()
	TObjectPtr<class ULockOn> m_lockon;
};
