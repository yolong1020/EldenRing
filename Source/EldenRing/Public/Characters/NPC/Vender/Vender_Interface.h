// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/SaveDatas.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "Vender_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVender_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IVender_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetVenderName(const FString& Name) PURE_VIRTUAL(IVender_Interface::SetVenderName, );
	virtual const FString GetVenderName() const PURE_VIRTUAL(IVender_Interface::GetVenderName, return FString(); );
	
	virtual void SetTradeItems(const TArray<FInventorySaveData>& Items) PURE_VIRTUAL(IVender_Interface::SetTradeItems, );
	virtual TArray<TObjectPtr<class UItemObject>>* const GetTradeItem() PURE_VIRTUAL(IVender_Interface::GetTradeItem, return nullptr; );


	virtual void SetDialouge(const FCommunicateScriptData& Dialouge) PURE_VIRTUAL(IVender_Interface::SetDialouge, );
	virtual void SetCommunicateIndex(const int32& Idx) PURE_VIRTUAL(IVender_Interface::SetCommunicateIndex, );
	virtual void SetCameras(const TArray<FString>& Cameras) PURE_VIRTUAL(IVender_Interface::SetCameras, );
	virtual void SetCurrentPoint(const FString& Point) PURE_VIRTUAL(IVender_Interface::SetCurrentPoint, );

	virtual const TObjectPtr<UDataTable> GetDialougeTable() const PURE_VIRTUAL(IVender_Interface::GetDialougeTable, return nullptr; );
	virtual const FCommunicateScriptData GetDialouge() const PURE_VIRTUAL(IVender_Interface::GetDialouge, return FCommunicateScriptData(); );
	virtual const int32 GetCommunicateIndex() const PURE_VIRTUAL(IVender_Interface::GetCommunicateIndex, return 0; );
	virtual TArray<FString> const GetCameras() const PURE_VIRTUAL(IVender_Interface::GetCameras, return TArray<FString>(); );
	virtual const FString GetCurrentPointName() const PURE_VIRTUAL(IVender_Interface::GetCurrentPointName, return FString(); );
};
