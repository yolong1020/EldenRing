// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/BaseLevelScript.h"
#include "MainTitle.generated.h"

UCLASS()
class ELDENRING_API AMainTitle : public ABaseLevelScript
{
	GENERATED_BODY()
	
public:
	virtual void LoadedLevel() override final {};
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	TObjectPtr<class UTitle> m_widget;
};
