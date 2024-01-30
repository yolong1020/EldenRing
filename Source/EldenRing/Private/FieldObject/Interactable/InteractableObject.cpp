// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Interactable/InteractableObject.h"
#include "Kismet/GameplayStatics.h"
#include "GameBase/ERGameInstance.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "System/NPCInteractionManager.h"
#include "System/ObserverManager.h"
#include "Macro/DebugMacros.h"

AInteractableObject::AInteractableObject() : m_is_first(true)
{
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = m_mesh;

	m_sphere_interaction = CreateDefaultSubobject<USphereComponent>(FName("Interact Sphere"));
	CHECK_INVALID_PTR(m_sphere_interaction)
	m_sphere_interaction->SetupAttachment(RootComponent);

	m_sphere_interaction->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_sphere_interaction->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_sphere_interaction->SetGenerateOverlapEvents(true);

	m_box_interaction = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Box"));
	CHECK_INVALID_PTR(m_box_interaction)
	m_box_interaction->SetupAttachment(RootComponent);

	m_box_interaction->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_box_interaction->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_box_interaction->SetGenerateOverlapEvents(true);
}

void AInteractableObject::Destroyed()
{
	Super::Destroyed();

	if (m_observer_mgr.IsValid()) { m_observer_mgr->UnregistObserverObject(this); }
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("InteractObject"));

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_observer_mgr->RegistObserver<AInteractableObject>(EObserverEventType::EOET_SaveData, this, &AInteractableObject::SaveInteractObjectData);
}

void AInteractableObject::SaveInteractObjectData()
{
	UERGameInstance* instance = GetGameInstance<UERGameInstance>();
	CHECK_INVALID_PTR(instance)

	instance->SaveInteractObject(this);
}

