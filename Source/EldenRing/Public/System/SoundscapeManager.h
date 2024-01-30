// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundscapeManager.generated.h"

UCLASS(Blueprintable)
class ELDENRING_API USoundscapeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void PlayStageBGM(USoundBase* const Sound, const float FadeIn = 1, const float FadeOut = 1, const bool AutoDestory = true);
	void StopStageBGM(const float FadeIn = 1, const float FadeOut = 1, const bool AutoDestory = true);

	void PlayNofityBGM(USoundBase* const Sound, const bool StopStageBGM, const float FadeIn = 1, const float FadeOut = 1);
	void PlayBossBGM(USoundBase* const Sound, const float FadeIn = 1, const float FadeOut = 1);
	void ChangePhaseBGM();

	UFUNCTION()
	void ReplayStageBGM();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;
	virtual void Deinitialize() override final;

private:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> m_stage_bgm;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> m_boss_bgm;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> m_notify_bgm;
};
