// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BaseGameState.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Item.h"
#include "Items/Treasure.h"
#include "Items/ItemObject.h"
#include "Basic/DebugMacros.h"

void ABaseGameState::SpawnItemFromActor(AActor* actor, UItemObject* item_object, const bool& is_ground_clamp)
{
	if (nullptr == actor) return;

	FVector location = actor->GetActorLocation();
	FVector forward  = actor->GetActorForwardVector();

	TArray<AActor*> ignores;
	FHitResult hit_result;
	FVector	   spawn_location = location + (forward * 150.f);

	bool result = false;
	if (is_ground_clamp)
	{
		result = UKismetSystemLibrary::LineTraceSingle(this,
			spawn_location, 
			FVector(spawn_location.X, spawn_location.Y, spawn_location.Z - 1000),
			TraceTypeQuery1, false, 
			ignores, 
			EDrawDebugTrace::None, 
			hit_result, 
			true);

		if (result) { spawn_location = hit_result.Location; }
	}

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	AItem* spawned_item = world->SpawnActor<AItem>(item_object->GetItemClass(), spawn_location, FRotator::ZeroRotator);
	CHECK_INVALID(spawned_item)

	spawned_item->SetSphereCollisionActivation(ECollisionEnabled::QueryOnly);
}

void ABaseGameState::SpawnGoldFromActor(AActor* actor, const int32& gold_amount, const bool& is_ground_clamp)
{
	if (nullptr == actor) return;

	FVector location = actor->GetActorLocation();
	FVector forward  = actor->GetActorForwardVector();

	TArray<AActor*> ignores;
	FHitResult hit_result;
	FVector	   spawn_location = location + (forward * 150.f);

	bool result = false;
	if (is_ground_clamp)
	{
		result = UKismetSystemLibrary::LineTraceSingle(this,
			spawn_location,
			FVector(spawn_location.X, spawn_location.Y, spawn_location.Z - 1000),
			TraceTypeQuery1, false,
			ignores,
			EDrawDebugTrace::None,
			hit_result,
			true);

		if (result) { spawn_location = hit_result.Location; }
	}

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	AItem* spawned_item = world->SpawnActor<AItem>(m_gold_class, spawn_location, FRotator::ZeroRotator);
	CHECK_INVALID(spawned_item)
	spawned_item->SetSphereCollisionActivation(ECollisionEnabled::QueryOnly);

	ATreasure* gold = Cast<ATreasure>(spawned_item);
	CHECK_INVALID(gold)
	gold->InitDroppedTreasureFromPlayer(gold_amount);
}
