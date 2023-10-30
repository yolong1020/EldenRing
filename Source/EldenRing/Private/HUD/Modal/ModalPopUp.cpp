// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Modal/ModalPopUp.h"
#include "HUD/Function/InputCounter.h"
#include "Characters/Players/C0000.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetTextLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Basic/DebugMacros.h"

void UModalPopUp::InitModal(const FString& title)
{
	CHECK_INVALID(ModalTitle)

	ModalTitle->SetText(FText::FromString(title));

	ButtonConfirm->OnClicked.AddDynamic(this, &UModalPopUp::OnClickConfirm);
	ButtonCancel->OnClicked.AddDynamic(this, &UModalPopUp::OnClickCancel);

	CHECK_INVALID(InputCounter)
	InputCounter->InitInputCounter();
}

void UModalPopUp::OnClickConfirm()
{
	AC0000* player = GetOwningPlayerPawn<AC0000>();
	CHECK_INVALID(player)

	CHECK_INVALID(InputCounter)
	int32 input_amount = InputCounter->GetTextAmount();

	if (player->GetTotalGold() >= input_amount)
	{
		ABaseGameState* game_state = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
		CHECK_INVALID(game_state)
			
		player->SetGold(-input_amount);
		game_state->SpawnGoldFromActor(player, input_amount, true);

		this->RemoveFromParent();
	}
}

void UModalPopUp::OnClickCancel()
{
	this->RemoveFromParent();
}
