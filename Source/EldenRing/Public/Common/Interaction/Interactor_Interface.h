// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractorController.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "Interactor_Interface.generated.h"

class IInteractorController;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractor_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IInteractor_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const TObjectPtr<class ACameraActor> GetCameraActor() const PURE_VIRTUAL(IInteractor_Interface::GetCameraActor, return nullptr; );
	virtual const FString GetInteractorName() const PURE_VIRTUAL(IInteractor_Interface::GetInteractorName(), return FString(); );
	virtual const FString GetOptionKey() const PURE_VIRTUAL(IInteractor_Interface::GetOptionKey(), return FString(); );

	virtual void SetFirstInteract(const bool IsFirst) PURE_VIRTUAL(IInteractor_Interface::SetFirstInteract, );
	virtual void SetOptionKey(const FString& Key) PURE_VIRTUAL(IInteractor_Interface::SetOptionKey, );
	virtual const bool IsFirstInteract() const PURE_VIRTUAL(IInteractor_Interface::IsFirstInteract, return false; );
	virtual void DoFirstInteract() PURE_VIRTUAL(IInteractor_Interface::DoFirstInteract, );

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteractWith(const TScriptInterface<IInteractorController>& Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BeginInteraction(const TScriptInterface<IInteractorController>& Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CommitInteraction(const TScriptInterface<IInteractorController>& Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CancelInteraction(const TScriptInterface<IInteractorController>& Interactor);
};
