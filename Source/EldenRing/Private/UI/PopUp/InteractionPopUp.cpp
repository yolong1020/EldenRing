// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PopUp/InteractionPopUp.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Input/IconDataAsset.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Communicate/CommunicateCallOut.h"
#include "UI/Notify/LostGraceDiscovered.h"
#include "UI/Notify/EnemyFelled.h"
#include "UI/BoneFire/BoneFireMenu.h"
#include "GameFramework/Character.h"
#include "System/NPCInteractionManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "Characters/Players/C0000.h"
#include "FieldObject/Interactable/Bonefire.h"
#include "Macro/DebugMacros.h"

void UInteractionPopUp::ShowPopUp(const TObjectPtr<APlayerController>& Controller, const EInteractPopupType& Type, const bool& IsFirst)
{
	if (!Controller || IsPlayingAnimation() || IsInViewport()) return;

	CHECK_INVALID_PTR(IconData)
	TArray<FIconData>* arr = IconData->GetIconDataArr();
	CHECK_INVALID_PTR(arr)

	m_type = Type;

	int32 idx = 0;
	switch (Type)
	{
		case EInteractPopupType::EIPT_ToNPC: {
			Desc->SetText(FText::FromString(TEXT("말을 건다")));
			idx = (int32)EInputActionType::EIAT_Interact;
		}break;
		case EInteractPopupType::EIPT_ToBonefire: {
			Desc->SetText(FText::FromString(IsFirst ? TEXT("축복에 닿는다") : TEXT("축복에서 쉰다")));
			idx = (int32)EInputActionType::EIAT_Interact;
		}break;
		case EInteractPopupType::EIPT_ToBossArea: {
			Desc->SetText(FText::FromString(TEXT("안개 속으로 들어간다")));
			idx = (int32)EInputActionType::EIAT_Interact;
		}break;
	}

	if (arr->IsValidIndex(idx)) {
		Icon->SetBrushFromMaterial((*arr)[idx].MaterialInstance);
	}

	m_controller = Controller;
	CHECK_INVALID_PTR(m_controller->InputComponent)
	m_controller->InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, this, &UInteractionPopUp::OnInteract);

	PlayAnimation(StartShowPopUp);
	m_is_enable_input = false;

	AddToViewport();
}

void UInteractionPopUp::HidePopUp()
{
	StopAllAnimations();
	PlayAnimation(StartHidePopUp);
}

void UInteractionPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	UClass* discovered_class = LoadClass<ULostGraceDiscovered>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Notify/WBP_LostGraceDiscovered.WBP_LostGraceDiscovered_C'"));
	CHECK_INVALID_PTR(discovered_class)
	m_discovered = CreateWidget<ULostGraceDiscovered>(world, discovered_class);
	CHECK_INVALID_PTR(m_discovered)

	UClass* bonefire_class = LoadClass<UBoneFireMenu>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Bonefire/WBP_BonefireMenu.WBP_BonefireMenu_C'"));
	CHECK_INVALID_PTR(bonefire_class)
	m_bonefire_menu = CreateWidget<UBoneFireMenu>(world, bonefire_class);
	CHECK_INVALID_PTR(m_bonefire_menu)

	UGameInstance* instance = UGameplayStatics::GetGameInstance(world);
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)

	FWidgetAnimationDynamicEvent show_event;
	show_event.BindDynamic(this, &UInteractionPopUp::EnableInput);
	BindToAnimationFinished(StartShowPopUp, show_event);
	FWidgetAnimationDynamicEvent hide_event;
	hide_event.BindDynamic(this, &UInteractionPopUp::ClosePopUp);
	BindToAnimationFinished(StartHidePopUp, hide_event);
}

void UInteractionPopUp::OnInteract()
{
	CHECK_INVALID_SMART_PTR(m_controller)
	if (!m_is_enable_input) return;

	PlayAnimation(StartHidePopUp);

	switch (m_type)
	{
	case EInteractPopupType::EIPT_ToNPC:
		{
			m_interact_mgr->SetEnableFramingCamera(true, 1.0f);
			m_interact_mgr->OpenCommunicate();
		} break;
	case EInteractPopupType::EIPT_ToBox:
		break;
	case EInteractPopupType::EIPT_ToBonefire:
		{
			AC0000* player = Cast<AC0000>(m_controller->AcknowledgedPawn);
			CHECK_INVALID_PTR(player)

			if (m_interact_mgr->IsFirstInteract())
			{
				m_discovered->StartDirecting(m_controller.Get());

				m_interact_mgr->DoFirstInteract();
				IInteractor_Interface* bonefire = Cast<IInteractor_Interface>(m_interact_mgr->GetInteractorObject());
				CHECK_INVALID_PTR(bonefire)

				player->OnDiscovered(bonefire->GetInteractorName());
			}
			else
			{
				IInteractor_Interface* bonefire = Cast<IInteractor_Interface>(m_interact_mgr->GetInteractorObject());
				CHECK_INVALID_PTR(bonefire)

				m_bonefire_menu->ShowBonefireMenu(m_controller.Get());
				player->OnRest(bonefire->GetInteractorName());
			}
		} break;
	case EInteractPopupType::EIPT_ToBossArea:
		{
			AC0000* player = Cast<AC0000>(m_controller->AcknowledgedPawn);
			CHECK_INVALID_PTR(player)

			m_interact_mgr->DoFirstInteract();
			player->OnEnterBossRoom();
		} break;
	case EInteractPopupType::EIPT_Max:
		break;
	default:
		break;
	}
}

void UInteractionPopUp::EnableInput()
{
	m_is_enable_input = true;
}

void UInteractionPopUp::ClosePopUp()
{
	CHECK_INVALID_SMART_PTR(m_controller)
	CHECK_INVALID_PTR(m_controller->InputComponent)
	m_controller->InputComponent->KeyBindings.Empty();

	RemoveFromParent();
}
