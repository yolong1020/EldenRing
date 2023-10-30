// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/GameHUD_Actor.h"
#include "HUD/Status/GameHUD.h"
#include "Basic/DebugMacros.h"

void AGameHUD_Actor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID(PlayerOwner)
	CHECK_INVALID(m_widget_class)

	m_widget_hud = CreateWidget<UGameHUD>(PlayerOwner, m_widget_class);
	m_widget_hud->AddToViewport();
}
