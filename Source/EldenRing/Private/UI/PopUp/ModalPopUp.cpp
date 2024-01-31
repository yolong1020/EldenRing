// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PopUp/ModalPopUp.h"
#include "UI/Function/InputCounter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameBase/ERGameState.h"
#include "System/StorageManager.h"
#include "System/UIManager.h"
#include "Characters/Players/C0000.h"
#include "Macro/DebugMacros.h"

void UModalPopUp::InitModal(const FString& title)
{
	CHECK_INVALID_PTR(ModalTitle)

	ModalTitle->SetText(FText::FromString(title));

	ButtonConfirm->OnClicked.AddDynamic(this, &UModalPopUp::OnClickConfirm);
	ButtonCancel->OnClicked.AddDynamic(this, &UModalPopUp::OnClickCancel);

	CHECK_INVALID_PTR(InputCounter)
	InputCounter->InitInputCounter();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
}

FReply UModalPopUp::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (IsInViewport() && EKeys::Escape == InKeyEvent.GetKey())
	{
		m_ui_mgr->UnregistUI();
	}

	return result;
}

void UModalPopUp::OnClickConfirm()
{
	AC0000* player = GetOwningPlayerPawn<AC0000>();
	CHECK_INVALID_PTR(player)

	CHECK_INVALID_PTR(InputCounter)
	int32 input_amount = InputCounter->GetTextAmount();

	if (player->GetTotalGold() >= input_amount)
	{
		AERGameState* game_state = Cast<AERGameState>(UGameplayStatics::GetGameState(this));
		CHECK_INVALID_PTR(game_state)
			
		player->SetGold(-input_amount);
		m_storage_mgr->TryUpdateGold(player->GetTotalGold());

		game_state->SpawnGoldFromObject(player, input_amount, true);

		CHECK_INVALID_SMART_PTR(m_ui_mgr)
		m_ui_mgr->UnregistUI();
	}
}

void UModalPopUp::OnClickCancel()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->UnregistUI();
}
