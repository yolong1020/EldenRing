// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LockOn/LockOn.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/GameCharacter.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Macro/DebugMacros.h"

void ULockOn::SetPosition(const TWeakObjectPtr<APlayerController> Controller, const TWeakObjectPtr<AGameCharacter> Target)
{
	CHECK_INVALID_SMART_PTR(Target)
	CHECK_INVALID_SMART_PTR(Controller)

	m_target	 = Target;
	m_controller = Controller;

	FVector2D view_position = FVector2D::ZeroVector;
	bool convert_result = m_controller->ProjectWorldLocationToScreen(m_target->GetBoneLocation("Spine2", EBoneSpaces::WorldSpace), view_position);

	float half = SizeBox->GetWidthOverride() * 0.5f;
	SetPositionInViewport(FVector2D(view_position.X - half, view_position.Y - half));

	if (!IsInViewport()) { AddToViewport(); }
}
