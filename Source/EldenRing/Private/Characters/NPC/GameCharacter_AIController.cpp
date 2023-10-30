// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/GameCharacter_AIController.h"
#include "Characters/NPC/NPC_Character.h"

void AGameCharacter_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	ANPC_Character* character = Cast<ANPC_Character>(GetCharacter());
	if (nullptr == character) return;

	character->OnMoveCompleted(Result);
}
