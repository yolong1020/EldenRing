// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameObjectManager.generated.h"

UCLASS()
class ELDENRING_API UGameObjectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void RegistObject(TSubclassOf<AActor> ObjectType, const TObjectPtr<AActor>& Object);

private:
	TMap<TSubclassOf<AActor>, TArray<TObjectPtr<AActor>>> m_container;
};
