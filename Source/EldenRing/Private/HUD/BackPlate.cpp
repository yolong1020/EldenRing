// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BackPlate.h"
#include "Components/Border.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameBase/ERGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BorderSlot.h"
#include "Items/ItemObject.h"
#include "Characters/Players/C0000.h"
#include "Macro/DebugMacros.h"

bool UBackPlate::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (nullptr == InOperation) return result;

	TObjectPtr<UItemObject> item = Cast<UItemObject>(InOperation->Payload);

	AERGameState* game_state = Cast<AERGameState>(UGameplayStatics::GetGameState(this));
	if (nullptr == game_state) return result;

	AC0000* player = GetOwningPlayerPawn<AC0000>();
	if (nullptr == player) return result;

	game_state->SpawnItemFromObject(player, item, true);
	return result;
}
