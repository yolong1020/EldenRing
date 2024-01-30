// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/SaveDatas.h"
#include "Player_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayer_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IPlayer_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetDefaultItems() PURE_VIRTUAL(IPlayer_Interface::SetDefaultItems, );

	virtual const TObjectPtr<class APlayerController> GetControllerComponent() PURE_VIRTUAL(IPlayer_Interface::GetInputComponent, return nullptr; );
	virtual class UGameHUDComponent* const GetHUDComponent() const PURE_VIRTUAL(IPlayer_Interface::GetHUDComponent, return nullptr; );
};
