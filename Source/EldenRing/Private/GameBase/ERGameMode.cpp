// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/ERGameMode.h"
#include "GameBase/ERGameState.h"
#include "GameBase/ERGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "System/SoundscapeManager.h"
#include "System/UIManager.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "Characters/Players/C0000.h"
#include "Characters/Players/ERPlayerStart.h"
#include "Levels/BaseLevelScript.h"
#include "HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Console.h"
#include "Macro/DebugMacros.h"

AERGameMode::AERGameMode()
{
	GameStateClass			= AERGameState::StaticClass();
	PlayerControllerClass	= APlayerController::StaticClass();
	DefaultPawnClass		= AC0000::StaticClass();
	HUDClass				= UGameHUD::StaticClass();
}

APawn* AERGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	if (instance && instance->IsLevelLoadComplete())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
		UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
		APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
		if (!ResultPawn)
		{
			UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
		return ResultPawn;
	}
	else
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (controller)
		{
			controller->bShowMouseCursor = true;
		}
		return nullptr;
	}
}

void AERGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("[GameMode InitGame]"))

	if (MapName.Contains("GameField"))
	{
		UGameInstance* instance = GetGameInstance();
		CHECK_INVALID_PTR(instance)
		UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
		CHECK_INVALID_PTR(ui_mgr)
		ui_mgr->ShowBlackOutNoAnimation();
	}
}

void AERGameMode::RequestRestartPlayer(TObjectPtr<AController> NewPlayer)
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Player"), players);

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	if (instance->IsNewGame()) { instance->SetAsNewGame(false); }

	USoundscapeManager* sound_mgr =	instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->ReplayStageBGM();

	if (players.IsEmpty())
	{
		instance->LoadLevelObjects();
	}

	Super::RestartPlayer(NewPlayer);
}
