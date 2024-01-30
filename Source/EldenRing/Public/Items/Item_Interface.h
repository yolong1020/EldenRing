// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IItem_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FadeOutItem(const float Duration) PURE_VIRTUAL(IItem_Interface::FadeOutItem, );
};
