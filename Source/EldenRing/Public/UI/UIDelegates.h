// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UItemObject;
struct FInventoryTile;

DECLARE_DELEGATE(FRefreshInteractionGrid);
DECLARE_DELEGATE(FTryUnequipItem);
DECLARE_DELEGATE_OneParam(FTryEquipItem, TObjectPtr<UItemObject>);
DECLARE_DELEGATE_OneParam(FHUDGoldUpdate, const int32&);

DECLARE_DELEGATE(FPopupActiveBtn);
DECLARE_DELEGATE(FPopupCancelBtn);
DECLARE_DELEGATE(FPopupConfirmlBtn);

DECLARE_DELEGATE_RetVal_TwoParams(bool, FTradeItemAdd, bool, TObjectPtr<UItemObject>);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FTradeItemRemove, bool, TObjectPtr<UItemObject>);

DECLARE_DELEGATE(FSelectButtonFunc)

DECLARE_DELEGATE_OneParam(FActivateMenuButtonFunc, const bool)
DECLARE_DELEGATE_RetVal(bool, FCheckActivateMenuButtonFunc)
DECLARE_DELEGATE_RetVal(bool, FCHeckAnimMenuButtonFunc)

DECLARE_DELEGATE(FBoneFireButtonFunc)
DECLARE_DELEGATE(FBoneFireEndFunc)

DECLARE_DELEGATE(FAfterShowBlackOutFunc)
DECLARE_DELEGATE(FAfterHideBlackOutFunc)

DECLARE_DELEGATE(FAfterPlayerDeathFunc)
