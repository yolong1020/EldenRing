// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialVolumeBox_GroupCall.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/UIManager.h"
#include "GameBase/ERGameInstance.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

ATutorialVolumeBox_GroupCall::ATutorialVolumeBox_GroupCall()
{
	m_slides.SetNum(3);
}

void ATutorialVolumeBox_GroupCall::SetCallOut(const TArray<FTutorialPersistentData>& CallOuts)
{
	m_slides.Empty(3);

	for (const FTutorialPersistentData& data : CallOuts) { m_slides.Add(data); }
}

void ATutorialVolumeBox_GroupCall::ReadyUp()
{
	FLatentActionInfo action;
	action.CallbackTarget	 = this;
	action.Linkage			 = 0;
	action.ExecutionFunction = FName("ClearGroupCall");

	float duration = TutorialNotifyDuration;
	UKismetSystemLibrary::Delay(GetWorld(), duration, action);
}

void ATutorialVolumeBox_GroupCall::OnBoxBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUICreated) return;

	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->CloseAllTutorial();

	SpawnGroupCall();
}

void ATutorialVolumeBox_GroupCall::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ClearGroupCall();
}

void ATutorialVolumeBox_GroupCall::AutoStart()
{
	SpawnGroupCall();
}

void ATutorialVolumeBox_GroupCall::SaveTutorial()
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	instance->SaveTutorialObject(this);
}

void ATutorialVolumeBox_GroupCall::SpawnGroupCall()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->OpenTutorialGroupCall(m_slides);
	ReadyUp();

	bUICreated = true;
}

void ATutorialVolumeBox_GroupCall::ClearGroupCall()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->CloseTutorialGroupCall();

	Destroy();
}