// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimeLineComponent.h"
#include "../../Components/AttributeComponent.h"
#include "../../Characters/CharacterTypes.h"
#include "../../Items/ItemTypes.h"
#include "QuickSlotTypes.h"
#include "GameHUD.generated.h"

#define MAX_BAR_WIDTH 1446;

class UOverlay;
class UProgressBar;
class USlider;
class UCurveFloat;
class UAttributeComponent;
class UItemObject;
class UImage;

UCLASS()
class BASIC_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitialStatusBar(UAttributeComponent* const attribute);

	UFUNCTION()
	void RefreshStatBar(const EOverlayStatType& type, const float& percent, const bool& is_regenereate);

	void SetQuickSlotItem(const EQuickSlotType& slot_type, UItemObject* const item_object = nullptr, const EPotionSizeType& potion_size = EPotionSizeType::EPST_Empty);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnUpdateHealthStatusBar(float value);

	UFUNCTION()
	void OnUpdateManaStatusBar(float value);

	UFUNCTION()
	void OnUpdateStaminaStatusBar(float value);

	void UpdateStatusTimeline(const EOverlayStatType& type);

private:
	UAttributeComponent*	m_attribute;
	FOnRefreshStatBar		m_regenerate_delegate;
#pragma region Health
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* HealthBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* HealthConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* HealthBarCurrent;
	
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	USlider* HealthCurrent;
#pragma endregion

#pragma region Mana
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* ManaBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* ManaConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* ManaBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	USlider* ManaCurrent;
#pragma endregion

#pragma region Stamina
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* StaminaBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* StaminaConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* StaminaBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	USlider* StaminaCurrent;
#pragma endregion

#pragma region QuickSlot
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* QuickSlot_Top_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* QuickSlot_Bottom_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* QuickSlot_Left_Item;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* QuickSlot_Right_Item;
#pragma endregion

#pragma region PotionMaterials
	TArray<UMaterialInterface*> m_potion_matarials;
#pragma endregion

#pragma region Timeline
	FTimeline m_tl_consume_health;
	FTimeline m_tl_consume_mana;
	FTimeline m_tl_consume_stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_consume;
#pragma endregion
};
