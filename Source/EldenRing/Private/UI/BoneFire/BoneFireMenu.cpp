// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BoneFire/BoneFireMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Button/BoneFireButton.h"
#include "Characters/Players/C0000.h"
#include "System/NPCInteractionManager.h"
#include "System/UIManager.h"
#include "components/TextBlock.h"
#include "Macro/DebugMacros.h"

void UBoneFireMenu::ShowBonefireMenu(const TObjectPtr<APlayerController>& Controller)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	if (IsPlayingAnimation()) return;

	m_ui_mgr->RegistUI(this);
	LocationName->SetText(FText::FromString(m_interact_mgr->GetInteractorName()));
	PlayAnimation(StartEnterGrace);

	Controller->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
}

void UBoneFireMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)

	FWidgetAnimationDynamicEvent show_blackout;
	show_blackout.BindDynamic(this, &UBoneFireMenu::HideBlackOut);
	BindToAnimationFinished(StartEnterGrace, show_blackout);

	FWidgetAnimationDynamicEvent close_menu;
	close_menu.BindDynamic(this, &UBoneFireMenu::CloseMenu);
	BindToAnimationFinished(StartHideMenu, close_menu);

	FBoneFireButtonFunc btn_callback;
	btn_callback.BindLambda([&]()->void {
			CHECK_INVALID_PTR(m_controller)
			AC0000* player = Cast<AC0000>(m_controller->AcknowledgedPawn);
			CHECK_INVALID_PTR(player)
			player->OnRecover();
		});
	RecoverBtn->BindButtonFunc(btn_callback);

	btn_callback.Unbind();
	btn_callback.BindLambda([&]()->void { ExitMenu(); });
	LeaveBtn->BindButtonFunc(btn_callback);
}

FReply UBoneFireMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (EKeys::Escape == InKeyEvent.GetKey())
	{
		ExitMenu();
	}

	return result;
}

void UBoneFireMenu::HideBlackOut()
{
	PlayAnimation(StartOpenMenu);

	CHECK_INVALID_PTR(m_controller)
	AC0000* player = Cast<AC0000>(m_controller->AcknowledgedPawn);
	CHECK_INVALID_PTR(player)

	player->OnCameraShift(200, 10, -30, 0);
}

void UBoneFireMenu::CloseMenu()
{
	CHECK_INVALID_PTR(m_controller)
	m_controller->bShowMouseCursor = false;

	FLatentActionInfo delay_callback;
	delay_callback.CallbackTarget = this;
	delay_callback.Linkage = 0;
	delay_callback.ExecutionFunction = FName("OpenInteractPopUp");
	UKismetSystemLibrary::Delay(GetWorld(), 1.5, delay_callback);
}

void UBoneFireMenu::OpenInteractPopUp()
{
	CHECK_INVALID_PTR(m_controller)
	m_interact_mgr->OpenInteractPopUp(m_controller, EInteractPopupType::EIPT_ToBonefire, false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller);

	m_ui_mgr->UnregistUI();
}

void UBoneFireMenu::ExitMenu()
{
	PlayAnimation(StartHideMenu);

	CHECK_INVALID_PTR(m_controller)
	AC0000* player = Cast<AC0000>(m_controller->AcknowledgedPawn);
	CHECK_INVALID_PTR(player)
	player->OnEndRest();
}
