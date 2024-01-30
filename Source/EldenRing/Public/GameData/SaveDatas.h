#pragma once

#include "Engine/DataTable.h"
#include "UI/Structure/InventoryTile.h"
#include "Characters/GameCharacter.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Characters/CharacterTypes.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "UI/Tutorial/TutorialVolumeBox.h"
#include "UI/Tutorial/TutorialData.h"
#include "SaveDatas.generated.h"

USTRUCT()
struct FInventorySaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FInventorySaveData() :
		RowName(""), 
		IsRotated(false)
	{}
	explicit FInventorySaveData(const FName& Name, const bool& Rotated, const FInventoryTile& Start) :
		RowName(Name),
		IsRotated(Rotated),
		Location(Start)
	{}
	~FInventorySaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryTile Location;
};

USTRUCT()
struct FEquipSaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FEquipSaveData() :
		RowName(""),
		IsRight(false)
	{}
	explicit FEquipSaveData(const FName& Name, const bool& Right) :
		RowName(Name),
		IsRight(Right)
	{}
	~FEquipSaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRight;
};

USTRUCT()
struct FGameObjectSaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FGameObjectSaveData() : 
		Transform(FTransform()),
		CommunicateIndex(-1),
		ActionState(0),
		WeaponEquipState(0),
		WaitTime_Min(0),
		WaitTime_Max(0),
		PatrolTime(0),
		KnowMissingTargetTime(0) {}
	~FGameObjectSaveData() {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGameCharacter> ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWeapon_Actor>> WeaponClassTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VenderName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OptionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AssemblyPointLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MovePointName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCommunicateScriptData CurrentDialouge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySaveData> TradeItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> CameraNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CommunicateIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ActionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 WeaponEquipState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime_Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime_Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnowMissingTargetTime;

};

USTRUCT()
struct FGameFieldObjectSaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FGameFieldObjectSaveData() : Transform(FTransform()) {}
	~FGameFieldObjectSaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ClassType;
};

USTRUCT()
struct FGameInteractorObjectSaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FGameInteractorObjectSaveData() : InteractorName(FString()), IsFirstInteract(true) {}
	~FGameInteractorObjectSaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFirstInteract;
};

USTRUCT()
struct FGameTutorialSaveData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FGameTutorialSaveData() :
		Transform(FTransform()) {}
	~FGameTutorialSaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATutorialVolumeBox> ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTutorialPersistentData> TutorialDatas;
};