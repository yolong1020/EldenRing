// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTableManager.generated.h"

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	EDTT_Item			UMETA(DisplayName = "Item Data"),
	EDTT_ItemProp		UMETA(DisplayName = "Item Properties"),
	EDTT_Weapon			UMETA(DisplayName = "Weapon Data"),
	EDTT_Vender			UMETA(DisplayName = "Vender List Data"),
	EDTT_Max			UMETA(DisplayName = "Max")
};

UCLASS()
class ELDENRING_API UDataTableManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDataTable* const GetDataTable(const EDataTableType& Type);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

private:
	UPROPERTY()
	TMap<EDataTableType, TObjectPtr<UDataTable>> m_data;
};
