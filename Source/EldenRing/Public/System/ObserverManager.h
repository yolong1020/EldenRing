// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/Event/EventTypes.h"
#include "ObserverManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FObserverEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FObserverDeathDelegate, TObjectPtr<class AGameCharacter>);

UCLASS()
class ELDENRING_API UObserverManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void TriggerEvent(const EObserverEventType& EventType);
	void TriggerDeathEvent(class AGameCharacter* const Target);

	template<class T>
	void RegistObserver(const EObserverEventType& EventType, T* const Observer, TDelegate<void()>::TMethodPtr<T> Func)
	{
		if (!m_event.IsValidIndex((int32)EventType)) return;

		FDelegateHandle handle = m_event[(int32)EventType].AddUObject(Observer, Func);
	}

	void UnregistObserver(const EObserverEventType& EventType)
	{
		if (!m_event.IsValidIndex((int32)EventType)) return;
		m_event[(int32)EventType].Clear();
	}

	template<class T>
	void RegistDeathObserver(class AGameCharacter* const Target, T* const Observer, TDelegate<void(TObjectPtr<class AGameCharacter>)>::TMethodPtr<T> Func)
	{
		if (!m_death_event.Contains(Target))
		{
			m_death_event.Add(Target, FObserverDeathDelegate());
		}
		FDelegateHandle handle = m_death_event[Target].AddUObject(Observer, Func);
	}

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

public:
	TArray<FObserverEvent> m_event;
	TMap<TObjectPtr<class AGameCharacter>, FObserverDeathDelegate> m_death_event;

	TMap<TObjectPtr<AActor>, TArray<TArray<FDelegateHandle>>> m_event_handle;
	TMap<TObjectPtr<AActor>, TArray<FDelegateHandle>>		  m_death_handle;
};
