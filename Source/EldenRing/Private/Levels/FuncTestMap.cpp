// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/FuncTestMap.h"
#include "GameBase/ERGameInstance.h"
#include "GameBase/ERGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Macro/DebugMacros.h"

void AFuncTestMap::BeginPlay()
{
	Super::BeginPlay();

	m_is_play = true;

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	instance->SetAsNewGame(true);
	instance->SetLoadState(ELevelLoadState::ELLS_Complete);

	UWorld* world = GetWorld();
	AERGameMode* game_mode = Cast<AERGameMode>(world->GetAuthGameMode());
	if (game_mode)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0);
		AActor* start = game_mode->FindPlayerStart(controller);
		if (start)
		{
			game_mode->SpawnDefaultPawnAtTransform(controller, start->GetActorTransform());
		}
	}
}
