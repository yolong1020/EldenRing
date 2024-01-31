// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleController.generated.h"

UCLASS()
class ELDENRING_API ATitleController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldShowMouseCursor() const override { return 1; };
};
