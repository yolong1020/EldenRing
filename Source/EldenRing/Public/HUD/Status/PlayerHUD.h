// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Characters/CharacterTypes.h"
#include "Items/ItemTypes.h"
#include "QuickSlotTypes.h"
#include "Components/TimeLineComponent.h"
#include "PlayerHUD.generated.h"

#define MAX_BAR_WIDTH 1446;

class UOverlay;
class UProgressBar;
class USlider;
class UCurveFloat;
class UAttributeComponent;
class UImage;

UCLASS()
class ELDENRING_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitialStatusBar(UAttributeComponent* const attribute);
	void UpdateStatusBar();

	void SetQuickSlotItem(const EQuickSlotType& Type, const TObjectPtr<class UItemObject> ItemObject, const EPotionSizeType& PotionSize = EPotionSizeType::EPST_Empty);

protected:
	virtual void NativeOnInitialized() override final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;

private:
	UFUNCTION()
	void StartHealthTL();

	UFUNCTION()
	void StartStaminaTL();

	UFUNCTION()
	void UpdateHealth(const float& Value);

	UFUNCTION()
	void UpdateStamina(const float& Value);

	UFUNCTION()
	void UpdateMana(const float& Value);

	void UpdateStat(const EOverlayStatType& Type);

	UFUNCTION()
	void UpdateRecoverHealth(const float& Value);

	UFUNCTION()
	void UpdateRecoverStamina(const float& Value);

	UFUNCTION()
	void UpdateConsumedHealth();

	UFUNCTION()
	void UpdateConsumedStamina(float Value);

	void UpdateConsumedStat(const EOverlayStatType& Type, const double& Consume, const double& Current);

private:
	UPROPERTY()
	TObjectPtr<UAttributeComponent> m_attribute;

#pragma region Health
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlay> HealthBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> HealthConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> HealthBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlider> HealthCurrent;
#pragma endregion

#pragma region Mana
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlay> ManaBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> ManaConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> ManaBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlider> ManaCurrent;
#pragma endregion

#pragma region Stamina
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlay> StaminaBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> StaminaConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> StaminaBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlider> StaminaCurrent;
#pragma endregion

#pragma region QuickSlot
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> QuickSlot_Top_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> QuickSlot_Bottom_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> QuickSlot_Left_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> QuickSlot_Right_Item;
#pragma endregion

#pragma region PotionMaterials
	UPROPERTY()
	TArray<TObjectPtr<class UMaterialInterface>> m_potion_matarials;
#pragma endregion

#pragma region Timeline
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> m_curve_consume;

	TArray<FTimerHandle> m_timer;
	TArray<FTimeline> m_tl_consume;
	TArray<bool> m_is_delay;
#pragma endregion
};
