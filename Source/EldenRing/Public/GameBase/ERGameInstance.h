// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Characters/Players/Player_Interface.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/GameCharacter_Interface.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "FieldObject/FieldObject_Interface.h"
#include "UI/Loading/Loading.h"
#include "UI/Tutorial/Tutorial_Interface.h"
#include "ERGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FLevelLoadCompleteEvent);

UENUM(BlueprintType)
enum class ELevelLoadState : uint8
{
	ELLS_Start		UMETA(DisplayName = "Start"),
	ELLS_Loading	UMETA(DisplayName = "Loading"),
	ELLS_Complete	UMETA(DisplayName = "Complete")
};

UCLASS()
class ELDENRING_API UERGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	const bool IsLevelLoadComplete() const { return m_load_state == ELevelLoadState::ELLS_Complete; }
	FORCEINLINE void SetLoadState(const ELevelLoadState& State) { m_load_state = State; }

	template<class T>
	void AddEventLevelLoadComplete(T* const Observer, TDelegate<void()>::TMethodPtr<T> Func)
	{
		m_event.AddUObject(Observer, Func);
	}
	void RequestLevelLoadCompleteEvent();

	void OpenLevel(const FString& LevelPath, const FName& LevelName);
	void LoadLevelObjects();

	void SavePlayer(const TScriptInterface<IGameCharacter_Interface>& Character_Interface);
	void LoadPlayer(const TScriptInterface<IPlayer_Interface>& Player_Interface);
	
	void SaveNPC(const TScriptInterface<INPC_Interfaces>& NPC_Interface);
	void LoadNPC();

	void SaveFieldObject(const TScriptInterface<IFieldObject_Interface>& FieldObject_Interface);
	void LoadFieldObject();

	void SaveInteractObject(const TScriptInterface<IInteractor_Interface>& Interactor_Interface);
	void LoadInteractObject();

	void SaveTutorialObject(const TScriptInterface<ITutorial_Interface>& Tutorial_Interface);
	void LoadTutorialObject();

	void ResetSaveData();

	FORCEINLINE void SetAsNewGame(const bool IsNew)		{ m_is_new_game = IsNew; }
	FORCEINLINE const bool IsNewGame()					{ return m_is_new_game; }

private:
	const TWeakObjectPtr<class UERSaveData> GetGameData();

private:

	FLevelLoadCompleteEvent m_event;

	ELevelLoadState m_load_state;
	bool			m_is_new_game;
};
