// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD_Actor.generated.h"

class UGameHUD;

UCLASS()
class BASIC_API AGameHUD_Actor : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	UGameHUD* GetOverlayHUD() const { return m_widget_hud; }

private:
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UGameHUD> m_widget_class;

	UGameHUD* m_widget_hud = nullptr;
};
