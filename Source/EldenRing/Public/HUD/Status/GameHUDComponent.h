// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameHUDComponent.generated.h"

class UGameHUD;

UCLASS()
class BASIC_API UGameHUDComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UGameHUD* m_widget_HUD;
};
