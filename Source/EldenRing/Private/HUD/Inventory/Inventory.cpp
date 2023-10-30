// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/Inventory.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Items/ItemObject.h"
#include "Input/Reply.h"
#include "Input/Events.h"
#include "Characters/Players/C0000.h"
#include "Basic/DebugMacros.h"

FReply UInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

bool UInventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drop_result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (nullptr == InOperation) return drop_result;

	UItemObject* item = Cast<UItemObject>(InOperation->Payload);

	ABaseGameState* game_state = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	if (nullptr == game_state) return drop_result;

	AC0000* player = GetOwningPlayerPawn<AC0000>();
	if (nullptr == player) return drop_result;

	game_state->SpawnItemFromActor(player, item, true);

	return true;
}
