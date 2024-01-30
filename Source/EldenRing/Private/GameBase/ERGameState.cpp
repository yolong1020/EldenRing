// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/ERGameState.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Item.h"
#include "Items/ItemObject.h"
#include "Items/Treasure/Treasure.h"
#include "Macro/DebugMacros.h"

void AERGameState::SpawnItemFromObject(APawn* Pawn, const TObjectPtr<UItemObject> ItemObject, const bool& IsGroundClamp)
{
	if (nullptr == Pawn) return;

	FVector location = Pawn->GetActorLocation();
	FVector forward  = Pawn->GetActorForwardVector();

	TArray<AActor*> ignores;
	FHitResult		hit_result;
	FVector			spawn_location = location + (forward * 50.f);

	bool result = false;
	if (IsGroundClamp)
	{
		int32 offset = 500;
		switch (ItemObject->GetItemEquipType())
		{
			case EEquipmentType::EET_Weapon: offset = 0;	 break;
			case EEquipmentType::EET_Arms:   offset = 1000;  break;
			case EEquipmentType::EET_Body:   offset = 1000;  break;
			case EEquipmentType::EET_Head:   offset = 2000;  break;
			case EEquipmentType::EET_Legs:   offset = 500;   break;
		}

		result = UKismetSystemLibrary::LineTraceSingle(this,
			spawn_location, 
			FVector(spawn_location.X, spawn_location.Y, spawn_location.Z - offset),
			TraceTypeQuery1, false, 
			ignores, 
			EDrawDebugTrace::None, 
			hit_result, 
			true);

		if (result) { spawn_location = hit_result.Location; }
	}

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	AItem* spawned_item = world->SpawnActor<AItem>(ItemObject->GetItemClass(), spawn_location, FRotator::ZeroRotator);
	CHECK_INVALID_PTR(spawned_item)

	spawned_item->SetSphereCollisionActivation(ECollisionEnabled::QueryOnly);
}

void AERGameState::SpawnGoldFromObject(APawn* Pawn, const int32& GoldAmount, const bool& IsGroundClamp)
{
	if (nullptr == Pawn) return;

	FVector location = Pawn->GetActorLocation();
	FVector forward  = Pawn->GetActorForwardVector();

	TArray<AActor*> ignores;
	FHitResult		hit_result;
	FVector			spawn_location = location + (forward * 150.f);

	bool result = false;
	if (IsGroundClamp)
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
	CHECK_INVALID_PTR(world)

	AItem* spawned_item = world->SpawnActor<AItem>(m_gold_class, spawn_location, FRotator::ZeroRotator);
	CHECK_INVALID_PTR(spawned_item)
	spawned_item->SetSphereCollisionActivation(ECollisionEnabled::QueryOnly);

	ATreasure* gold = Cast<ATreasure>(spawned_item);
	CHECK_INVALID_PTR(gold)
	gold->InitDroppedTreasureFromPlayer(GoldAmount);
}