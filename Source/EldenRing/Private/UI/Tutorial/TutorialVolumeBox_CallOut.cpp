// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialVolumeBox_CallOut.h"
#include "Characters/Players/C0000.h"
#include "Kismet/GameplayStatics.h"
#include "System/UIManager.h"
#include "GameBase/ERGameInstance.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

const TArray<FTutorialPersistentData> ATutorialVolumeBox_CallOut::GetCallOuts() const
{
	FTutorialPersistentData data(*UI_CTA_TEXT_1.ToString(), m_action, ETutorialPosition::ETP_Max, bIsLargeKey);
	TArray<FTutorialPersistentData> arr;
	arr.Add(data);

	return arr;
}

void ATutorialVolumeBox_CallOut::SetCallOut(const TArray<FTutorialPersistentData>& CallOuts)
{
	if (CallOuts.IsEmpty() || 1 != CallOuts.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid loaded tutorial data."))
		return;
	}

	UI_CTA_TEXT_1	= FText::FromName(CallOuts[0].Name);
	m_action	= CallOuts[0].Action;
	bIsLargeKey	= CallOuts[0].IsLargeKey;
}

void ATutorialVolumeBox_CallOut::ReadyUp()
{
	FLatentActionInfo action;
	action.CallbackTarget    = this;
	action.Linkage 		 = 0;
	action.ExecutionFunction = FName("ClearCallOut");

	float duration = TutorialNotifyDuration;
	UKismetSystemLibrary::Delay(GetWorld(), duration, action);
}

void ATutorialVolumeBox_CallOut::OnBoxBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUICreated) return;

	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->CloseAllTutorial();

	SpawnCallOut();
}

void ATutorialVolumeBox_CallOut::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ClearCallOut();
}

void ATutorialVolumeBox_CallOut::AutoStart()
{
	SpawnCallOut();
}

void ATutorialVolumeBox_CallOut::SaveTutorial()
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	instance->SaveTutorialObject(this);
}

void ATutorialVolumeBox_CallOut::SpawnCallOut()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->OpenTutorialCallout(*UI_CTA_TEXT_1.ToString(), m_action, bIsLargeKey);
	ReadyUp();

	bUICreated = true;
}

void ATutorialVolumeBox_CallOut::ClearCallOut()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->CloseTutorialCallout();

	Destroy();
}
