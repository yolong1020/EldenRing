// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LockOnComponent.generated.h"

class AGameCharacter;
class APlayerController;
class ULockOn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASIC_API ULockOnComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	ULockOnComponent();

	void ActiveLockOn(AActor* const target);
	void RefreshLockOn(AActor* const target);

protected:
	virtual void BeginPlay() override;

private:
	APlayerController* m_player_controller;

	UPROPERTY()
	ULockOn* m_widget_lockon;
};
