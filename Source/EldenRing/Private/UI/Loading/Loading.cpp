// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/Loading.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "GameBase/ERGameInstance.h"
#include "GameFramework/Character.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/LevelStreaming.h"
#include "System/UIManager.h"
#include "Levels/BaseLevelScript.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"
#include "WorldPartition/DataLayer/ActorDataLayer.h"
#include "Misc/PackageName.h"
#include "Macro/DebugMacros.h"

void ULoading::OpenLoading()
{
	m_is_triggered = false;
	AddToViewport();
}

void ULoading::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	bool is_complete = true;

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	const TArray<ULevelStreaming*> levels = world->GetStreamingLevels();
	if (levels.IsEmpty()) return;

	int32 complete = 0;
	for (ULevelStreaming* const level : levels)
	{
		if (!level->IsLevelLoaded()) is_complete = false;
		else ++complete;
	}

	float percent = complete / levels.Num();
	Progress->SetPercent(percent);
	Thumb->SetValue(percent);

	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();

	if (is_complete && !m_is_triggered && !ui_mgr->IsPlayingBalckout())
	{
		m_is_triggered = true;

		FAfterShowBlackOutFunc show_callback;	
		show_callback.BindLambda([&]()->void {

				if (IsInViewport()) { RemoveFromParent(); }



				UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
				CHECK_INVALID_PTR(instance)
				instance->SetLoadState(ELevelLoadState::ELLS_Complete);

				ABaseLevelScript* level = Cast<ABaseLevelScript>(GetWorld()->GetLevelScriptActor());
				CHECK_INVALID_PTR(level)
				level->LoadedLevel();
			});

		ui_mgr->ShowBlackOut(show_callback);
	}
}
