// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD_Actor.generated.h"

UCLASS()
class ELDENRING_API AGameHUD_Actor : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
