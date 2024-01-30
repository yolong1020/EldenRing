// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FieldObject_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFieldObject_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELDENRING_API IFieldObject_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetFieldObjectTransform(const FTransform& Transform) PURE_VIRTUAL(IFieldObject_Interface::SetFieldObjectTransform, );
	virtual const FTransform GetFieldObjectTransform() const PURE_VIRTUAL(IFieldObject_Interface::GetFieldObjectTransform, return FTransform(); );

	virtual TSubclassOf<class AFieldObject> GetClassType() const PURE_VIRTUAL(IFieldObject_Interface::GetClassType, return nullptr; );
};
