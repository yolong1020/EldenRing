// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BaseLevelScript.generated.h"

UCLASS()
class ELDENRING_API ABaseLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void LoadedLevel() PURE_VIRTUAL(ABaseLevelScript::LoadedLevel, );

	FORCEINLINE const FString&	GetSlotName() { return m_slot_name; }
	FORCEINLINE const bool		IsPlayLevel() { return m_is_play; }
	
protected:
	virtual void BeginPlay() override;
	virtual void LoadDataLayer();
	virtual void LoadNavDataLayer();

protected:
	UPROPERTY()
	TWeakObjectPtr<class USoundscapeManager> m_sound_mgr;

	FString m_slot_name;
	bool m_is_play;
};
