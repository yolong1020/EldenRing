// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/Tutorial/TutorialData.h"
#include "Tutorial_Interface.generated.h"

UINTERFACE(MinimalAPI)
class UTutorial_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API ITutorial_Interface
{
	GENERATED_BODY()

public:
	virtual const FTransform GetObjectTransform() const PURE_VIRTUAL(ITutorial_Interface::GetObjectTransform, return FTransform(); );
	virtual TSubclassOf<class ATutorialVolumeBox> GetClassType() const PURE_VIRTUAL(ITutorial_Interface::GetClassType, return nullptr; );
	virtual const TArray<FTutorialPersistentData> GetCallOuts() const PURE_VIRTUAL(ITutorial_Interface::GetCallOuts, return  TArray<FTutorialPersistentData>(); );

	virtual void SetObjectTransform(const FTransform& Transform) PURE_VIRTUAL(ITutorial_Interface::SetObjectTransform, );
	virtual void SetCallOut(const TArray<FTutorialPersistentData>& CallOuts) PURE_VIRTUAL(ITutorial_Interface::SetCallOut, );
};
