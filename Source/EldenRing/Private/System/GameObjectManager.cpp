// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameObjectManager.h"

void UGameObjectManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameObjectManager::Deinitialize()
{
	Super::Deinitialize();
}

void UGameObjectManager::RegistObject(TSubclassOf<AActor> ObjectType, const TObjectPtr<AActor>& Object)
{
	if (m_container.IsEmpty() || false == m_container.Contains(ObjectType))
	{
		TArray<TObjectPtr<AActor>> list;
		list.Add(Object);
		list.Shrink();

		m_container.Add(ObjectType, list);
	}
}
