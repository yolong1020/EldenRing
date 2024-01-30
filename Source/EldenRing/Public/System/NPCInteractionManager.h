// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/PopUp/PopUpTypes.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "NPCInteractionManager.generated.h"

UCLASS()
class ELDENRING_API UInteractionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FString  GetInteractorName() const { return m_interface->GetInteractorName(); }
	FORCEINLINE UObject* GetInteractorObject() const	 { return m_interface.GetObject(); }

	FORCEINLINE void SetInterface(const TScriptInterface<IInteractor_Interface>& Interactor) { m_interface = Interactor; }

	const bool CheckSameInteractPopUp(const EInteractPopupType& Type);

	void OpenInteractPopUp(const TObjectPtr<APlayerController>& Controller, const EInteractPopupType& Type, const bool& IsFirst = true);
	void CloseInteractPopUp();

	void OpenSelect(const uint8& FirstType, const uint8& SecondType);

	void OpenCommunicate();
	void OpenCommunicateByOption();
	void SetCurrentDialouge(const FCommunicateScriptData& Dialouge);
	UDataTable* const GetDialougeTable();
	const FCommunicateScriptData GetLastDialouge();

	void SetEnableFramingCamera(const bool& Enable, const float& BlendTime);
	const bool IsFirstInteract();
	void DoFirstInteract();

private:
	UPROPERTY()
	class UInteractionPopUp* m_interact_popup;

	UPROPERTY()
	class USelectionPopUp* m_selection_popup;

	UPROPERTY()
	class UCommunicateCallOut* m_communicate;

	UPROPERTY()
	AActor* m_prv_view_target;

	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;

	UPROPERTY()
	TScriptInterface<IInteractor_Interface> m_interface;
};
