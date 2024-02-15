// Fill out your copyright notice in the Description page of Project Settings.


#include "System/NPCInteractionManager.h"
#include "Characters/NPC/Vender/Vender.h"
#include "Characters/NPC/Vender/Vender_Interface.h"
#include "Camera/CameraActor.h"
#include "UObject/Class.h"
#include "UI/PopUp/InteractionPopUp.h"
#include "UI/PopUp/SelectionPopUp.h"
#include "UI/Button/ButtonType.h"
#include "UI/Communicate/CommunicateCallOut.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "UI/Notify/SubNotify.h"
#include "UI/Notify/EnemyFelled.h"
#include "Kismet/GameplayStatics.h"
#include "Input/InputActionType.h"
#include "Macro/DebugMacros.h"

const bool UInteractionManager::CheckSameInteractPopUp(const EInteractPopupType& Type)
{
	if (!m_interact_popup) return false;
	return m_interact_popup->GetPopUpType() == Type;
}

void UInteractionManager::OpenInteractPopUp(const TObjectPtr<APlayerController>& Controller, const EInteractPopupType& Type, const bool& IsFirst)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	if (Type == EInteractPopupType::EIPT_Max) return;

	if (!m_interact_popup)
	{
		UClass* type_class = LoadClass<UInteractionPopUp>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Popup/Interaction/WBP_InteractionPopup.WBP_InteractionPopup_C'"));
		CHECK_INVALID_PTR(type_class)

		m_interact_popup = CreateWidget<UInteractionPopUp>(GetWorld(), type_class);
		CHECK_INVALID_PTR(m_interact_popup)
	}
	m_interact_popup->ShowPopUp(Controller, Type, IsFirst);
}

void UInteractionManager::CloseInteractPopUp()
{
	if (!m_interact_popup) return;

	m_interact_popup->HidePopUp();
}

void UInteractionManager::OpenSelect(const uint8& FirstType, const uint8& SecondType)
{
	CHECK_INVALID_SMART_PTR(m_controller)

	const UEnum* enum_type = StaticEnum<ESelectPopupButtonType>();
	CHECK_INVALID_PTR(enum_type)

	if (!enum_type->IsValidEnumValue(static_cast<int64>(FirstType))		    ||
	    !enum_type->IsValidEnumValue(static_cast<int64>(SecondType))	    ||
	    (ESelectPopupButtonType)FirstType  == ESelectPopupButtonType::ESPBT_Max || 
	    (ESelectPopupButtonType)SecondType == ESelectPopupButtonType::ESPBT_Max) return;

	if (!m_selection_popup)
	{
		UClass* type_class = LoadClass<USelectionPopUp>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Popup/Selection/WBP_Selection.WBP_Selection_C'"));
		CHECK_INVALID_PTR(type_class)

		m_selection_popup = CreateWidget<USelectionPopUp>(m_controller.Get(), type_class);
		CHECK_INVALID_PTR(m_selection_popup)
	}

	m_selection_popup->SetOptionButtons(FirstType, SecondType);
	m_selection_popup->ShowPopUp(m_controller.Get());
}

void UInteractionManager::OpenCommunicate()
{
	CHECK_INVALID_SMART_PTR(m_controller)

	if (!m_communicate)
	{
		UClass* type_class = LoadClass<UCommunicateCallOut>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Communicate/WBP_CommunicateCallOut.WBP_CommunicateCallOut_C'"));
		CHECK_INVALID_PTR(type_class)

		m_communicate = CreateWidget<UCommunicateCallOut>(m_controller.Get(), type_class);
		CHECK_INVALID_PTR(m_communicate)
	}

	m_communicate->InitThreatenedDialogue();
	m_communicate->StartCommunicate(m_controller.Get());
}

void UInteractionManager::OpenCommunicateByOption()
{
	CHECK_INVALID_SMART_PTR(m_controller)

	if (!m_communicate)
	{
		UClass* type_class = LoadClass<UCommunicateCallOut>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Communicate/WBP_CommunicatCallOut.WBP_CommunicatCallOut_C'"));
		CHECK_INVALID_PTR(type_class)

		m_communicate = CreateWidget<UCommunicateCallOut>(m_controller.Get(), type_class);
		CHECK_INVALID_PTR(m_communicate)
	}

	if (!m_interface) return;
	m_communicate->InitThreatenedDialogue();
	m_communicate->StartOptionCommunicate(m_interface->GetOptionKey());
}

void UInteractionManager::SetEnableFramingCamera(const bool& Enable, const float& BlendTime)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerController* controller = Cast<APlayerController>(player->GetController());
	CHECK_INVALID_PTR(controller)

	if (Enable)
	{
		const ACameraActor* camera = m_interface->GetCameraActor();
		CHECK_INVALID_PTR(camera)

		controller->SetIgnoreLookInput(true);
		m_prv_view_target = controller->GetViewTarget();
		controller->SetViewTargetWithBlend(const_cast<AActor*>((Cast<AActor>(camera))), BlendTime, EViewTargetBlendFunction::VTBlend_Cubic, 2, true);
	}
	else
	{
		controller->SetIgnoreLookInput(false);
		AActor* target = IsValid(m_prv_view_target) ? m_prv_view_target : player;
		controller->SetViewTargetWithBlend(target, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic, 2, true);
	}

	FRotator rotator = controller->PlayerCameraManager->GetCameraRotation();
	controller->SetControlRotation(rotator);
}

void UInteractionManager::SetCurrentDialouge(const FCommunicateScriptData& Dialouge)
{
	if (!m_interface) return;

	IVender_Interface* vender_interface = Cast<IVender_Interface>(m_interface.GetObject());
	if (!vender_interface) return;

	vender_interface->SetDialouge(Dialouge);
}

UDataTable* const UInteractionManager::GetDialougeTable()
{
	if (!m_interface) return nullptr;

	IVender_Interface* vender_interface = Cast<IVender_Interface>(m_interface.GetObject());
	if (!vender_interface) return nullptr;

	return vender_interface->GetDialougeTable();
}

const FCommunicateScriptData UInteractionManager::GetLastDialouge()
{
	if (!m_interface) return FCommunicateScriptData();

	IVender_Interface* vender_interface = Cast<IVender_Interface>(m_interface.GetObject());
	if (!vender_interface) return FCommunicateScriptData();

	return vender_interface->GetDialouge();
}

const bool UInteractionManager::IsFirstInteract()
{
	if (!m_interface) return false;

	return m_interface->IsFirstInteract();
}

void UInteractionManager::DoFirstInteract()
{
	if (!m_interface) return;

	m_interface->DoFirstInteract();
}
