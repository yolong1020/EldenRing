// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Structure/InventoryTile.h"
#include "UI/UIDelegates.h"
#include "UI/Button/ButtonType.h"
#include "UI/PopUp/PopUpTypes.h"
#include "TradeManager.generated.h"

class UPopUp;
class UItemObject;
class AVender;

UCLASS()
class ELDENRING_API UTradeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void OpenTrade(const TObjectPtr<APlayerController>& Controller);

	FORCEINLINE void SetVenderData(AVender* const Vender)		{ m_vender = Vender; }
	FORCEINLINE AVender* const GetVender() const				{ return m_vender; }
	FORCEINLINE TArray<TObjectPtr<UItemObject>>* const GetVenderSlots() { return &m_vender_slots; }
	FORCEINLINE UPopUp* const GetPopup() const					{ return m_popup; }

	void InitTradeWidget(TArray<TObjectPtr<UItemObject>>* const PlayerSlots);
	void InitUpdateGoldCallback(const bool& IsPlayer, const FHUDGoldUpdate& Callback);
	void InitUpdateGridCallback(const bool& IsPlayer, const FRefreshInteractionGrid& Callback);

	void ShowItemInfo(const FVector2D& Position, const FVector2D& SlotSize, const TObjectPtr<UItemObject> ItemObject);
	void HideItemInfo();

	bool TryUpdateStorage();
	bool TryUpdateGold(const bool IsPlayer, const int32& TotalGold);
	bool TryAddItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject);
	bool TryAddItemAtIndex(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index);
	bool TryAddItemAtTile(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& StartTile);
	bool TryRemoveItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject);

	bool IsEmptyTile(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& Tile);
	bool IsEmptyIndex(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

private:
	const FInventoryTile IndexToTile(const int32& Index);
	const int32			 TileToIndex(const FInventoryTile& Tile);

	void AddItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index);
	void RemoveItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject);

private:
	UPROPERTY()
	TArray<TObjectPtr<UItemObject>>	 m_vender_slots;
	TArray<TObjectPtr<UItemObject>>* m_player_slots;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager>	m_ui_mgr;

	UPROPERTY()
	TObjectPtr<class UItemInfo>			m_item_info;

	UPROPERTY()
	TObjectPtr<class UPopUp>			m_popup;

	UPROPERTY()
	TObjectPtr<class UTrade>			m_widget_trade;

	UPROPERTY()
	TObjectPtr<class UTradeGrid>		m_widget_grid_player;

	UPROPERTY()
	TObjectPtr<class UTradeGrid>		m_widget_grid_vender;

	UPROPERTY()
	TObjectPtr<class APlayerController> m_controller;

	UPROPERTY()
	TObjectPtr<class AVender>			m_vender;

	FRefreshInteractionGrid	m_update_player;
	FRefreshInteractionGrid	m_update_vender;
	FHUDGoldUpdate			m_update_gold_player;
	FHUDGoldUpdate			m_update_gold_vender;

	int32	m_colum;
	int32	m_row;
};
