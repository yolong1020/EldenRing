// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialVolumeBox.h"
#include "UI/Tutorial/TutorialCallout.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "System/UIManager.h"
#include "System/ObserverManager.h"
#include "Characters/Players/C0000.h"
#include "Macro/DebugMacros.h"

ATutorialVolumeBox::ATutorialVolumeBox()
{
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(Volume);
}

void ATutorialVolumeBox::Destroyed()
{
	Super::Destroyed();

	UGameInstance* instance = GetGameInstance();
	if (instance)
	{
		UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
		if (observer_mgr) { observer_mgr->UnregistObserverObject(this); }
	}
}

const FTransform ATutorialVolumeBox::GetObjectTransform() const
{
	return GetTransform();
}

TSubclassOf<class ATutorialVolumeBox> ATutorialVolumeBox::GetClassType() const
{
	return GetClass();
}

void ATutorialVolumeBox::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("TutorialObject");

	if (bAutoStart) 
	 {
	 	FLatentActionInfo action;
	 	action.CallbackTarget = this;
	 	action.Linkage = 0;
	 	action.ExecutionFunction = FName("AutoStart");
	 	UKismetSystemLibrary::Delay(GetWorld(), AutoStartDelay, action);
	 }

	Volume->OnComponentBeginOverlap.AddDynamic(this, &ATutorialVolumeBox::OnBoxBeingOverlap);
	Volume->OnComponentEndOverlap.AddDynamic(this, &ATutorialVolumeBox::OnBoxEndOverlap);
	Volume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Volume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Volume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Volume->SetGenerateOverlapEvents(true);

	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	observer_mgr->RegistObserver<ATutorialVolumeBox>(EObserverEventType::EOET_SaveData, this, &ATutorialVolumeBox::SaveTutorial);
}