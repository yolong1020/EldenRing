// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundscapeManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AudioParameter.h"
#include "Macro/DebugMacros.h"

void USoundscapeManager::PlayStageBGM(USoundBase* const Sound, const float FadeIn, const float FadeOut, const bool AutoDestory)
{
	CHECK_INVALID_PTR(Sound)

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	if (m_stage_bgm)
	{
		m_stage_bgm->FadeOut(FadeOut, 0);
		m_stage_bgm->bAutoDestroy = AutoDestory;
	}

	m_stage_bgm = UGameplayStatics::CreateSound2D(world, Sound);
	CHECK_INVALID_PTR(m_stage_bgm)

	m_stage_bgm->VolumeMultiplier = 0.55;
	m_stage_bgm->bAllowSpatialization = false;
	m_stage_bgm->Play();
}

void USoundscapeManager::StopStageBGM(const float FadeIn, const float FadeOut, const bool AutoDestory)
{
	if (!m_stage_bgm) return;

	m_stage_bgm->FadeOut(FadeOut, 0);
	m_stage_bgm->bAutoDestroy = AutoDestory;
}

void USoundscapeManager::PlayNofityBGM(USoundBase* const Sound, const bool StopStageBGM, const float FadeIn, const float FadeOut)
{
	CHECK_INVALID_PTR(Sound)

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	if (StopStageBGM && m_stage_bgm)
	{
		m_stage_bgm->FadeOut(FadeOut, 0);
		m_stage_bgm->bAutoDestroy = false;
	}
	if (m_boss_bgm)
	{
		m_boss_bgm->FadeOut(FadeOut, 0);
		m_boss_bgm->bAutoDestroy = true;
	}

	m_notify_bgm = UGameplayStatics::CreateSound2D(world, Sound);
	CHECK_INVALID_PTR(m_notify_bgm)

	m_notify_bgm->VolumeMultiplier = 0.55;
	m_notify_bgm->bAllowSpatialization = false;
	m_notify_bgm->Play();
}

void USoundscapeManager::PlayBossBGM(USoundBase* const Sound, const float FadeIn, const float FadeOut)
{
	CHECK_INVALID_PTR(Sound)

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	if (m_stage_bgm)
	{
		m_stage_bgm->FadeOut(FadeOut, 0);
		m_stage_bgm->bAutoDestroy = false;
	}

	m_boss_bgm = UGameplayStatics::CreateSound2D(world, Sound);
	CHECK_INVALID_PTR(m_boss_bgm)

	m_boss_bgm->VolumeMultiplier = 0.55;
	m_boss_bgm->bAllowSpatialization = true;
	m_boss_bgm->Play();
}

void USoundscapeManager::ReplayStageBGM()
{
	if (m_boss_bgm)
	{
		m_boss_bgm->FadeOut(1.f, 0);
		m_boss_bgm->bAutoDestroy = true;
	}

	if (m_stage_bgm)
	{
		m_stage_bgm->VolumeMultiplier = 0.55;
		m_stage_bgm->bAllowSpatialization = false;
		m_stage_bgm->Play();
	}
}

void USoundscapeManager::ChangePhaseBGM()
{
	ISoundParameterControllerInterface* sound_interface = Cast<ISoundParameterControllerInterface>(m_boss_bgm);
	if (sound_interface)
	{
		sound_interface->SetTriggerParameter(FName("PhaseTrigger"));
	}
}

void USoundscapeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USoundscapeManager::Deinitialize()
{
}
