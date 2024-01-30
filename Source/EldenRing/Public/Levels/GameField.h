// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/BaseLevelScript.h"
#include "GameField.generated.h"

UCLASS()
class ELDENRING_API AGameField : public ABaseLevelScript
{
	GENERATED_BODY()
	
public:
	virtual void LoadedLevel() override final;
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void StartStage();

private:
	FTimerHandle timer_spawn;
	FTimerHandle timer_start;

	UPROPERTY()
	TObjectPtr<class ULoading> m_widget;

};
