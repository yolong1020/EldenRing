// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/FieldObject.h"
#include "GameBase/ERGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "System/ObserverManager.h"
#include "Macro/DebugMacros.h"

AFieldObject::AFieldObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFieldObject::Destroyed()
{
	Super::Destroyed();

	if (m_observer_mgr.IsValid()) { m_observer_mgr->UnregistObserverObject(this); }
}

void AFieldObject::SaveFieldObjectData()
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	instance->SaveFieldObject(this);
}

void AFieldObject::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("FieldObject"));

	m_start_transform = GetTransform();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)

	m_observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_SMART_PTR(m_observer_mgr)
	m_observer_mgr->RegistObserver<AFieldObject>(EObserverEventType::EOET_SaveData, this, &AFieldObject::SaveFieldObjectData);
}

