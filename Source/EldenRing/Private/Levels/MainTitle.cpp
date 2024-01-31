// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/MainTitle.h"
#include "UI/Title/Title.h"
#include "Kismet/GameplayStatics.h"
#include "System/SoundscapeManager.h"
#include "Macro/DebugMacros.h"

void AMainTitle::BeginPlay()
{
	Super::BeginPlay();

	m_is_play = false;

	UClass* widget_class = LoadClass<UTitle>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Title/WBP_Title.WBP_Title_C'"));
	CHECK_INVALID_PTR(widget_class)

	m_widget = CreateWidget<UTitle>(GetWorld(), widget_class);
	m_widget->AddToViewport();
	m_widget->ShowTitle();

	USoundBase* sound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/EldenRing/Sounds/BackgroundMusic/Title.Title'"));
	CHECK_INVALID_PTR(sound)
	m_sound_mgr->PlayStageBGM(sound);
}

void AMainTitle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!m_widget) return;
	m_widget->RemoveFromParent();
}
