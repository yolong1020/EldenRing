// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LockOn/LockOnComponent.h"
#include "HUD/LockOn/LockOn.h"
#include "Characters/GameCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/GameViewportSubsystem.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

#include "Basic/DebugMacros.h"

ULockOnComponent::ULockOnComponent()
{
}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* actor = Cast<ACharacter>(GetOwner());
	CHECK_INVALID(actor)

	m_player_controller = Cast<APlayerController>(actor->GetController());
	CHECK_INVALID(m_player_controller)

	if (nullptr == m_widget_lockon)
	{
		m_widget_lockon = Cast<ULockOn>(GetUserWidgetObject());
		CHECK_INVALID(m_widget_lockon)
	}
}

void ULockOnComponent::ActiveLockOn(AActor* const target)
{
	CHECK_INVALID(m_widget_lockon)

	if (m_widget_lockon->IsInViewport())
	{
		m_widget_lockon->RemoveFromParent();
	}
	else
	{
		m_widget_lockon->AddToViewport();
		CHECK_INVALID(m_widget_lockon->MarkImage);

		AGameCharacter* enemy = Cast<AGameCharacter>(target);
		CHECK_INVALID(enemy)

		FVector2D location_viewport;
		m_player_controller->ProjectWorldLocationToScreen(enemy->GetBoneLocation(FName("Spine")), location_viewport, true);

		UCanvasPanelSlot* canvas = UWidgetLayoutLibrary::SlotAsCanvasSlot(m_widget_lockon->MarkImage);
		if (canvas == nullptr)

		canvas->SetPosition(location_viewport);
	}
}

void ULockOnComponent::RefreshLockOn(AActor* const target)
{
	CHECK_INVALID(m_widget_lockon)
}
