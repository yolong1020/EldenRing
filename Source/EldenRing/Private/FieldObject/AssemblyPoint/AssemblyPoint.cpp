// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/NPC/NPC_Character.h"
#include "System/ObserverManager.h"
#include "Macro/DebugMacros.h"

AAssemblyPoint::AAssemblyPoint()
{
 	PrimaryActorTick.bCanEverTick = true;

	m_mesh			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent	= m_mesh;

	m_fire_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particle Component"));
	m_fire_particle->SetupAttachment(RootComponent);

	m_smoke_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Particle Component"));
	m_smoke_particle->SetupAttachment(RootComponent);

	m_capsule_block = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere"));
	m_capsule_block->SetupAttachment(RootComponent);
}

void AAssemblyPoint::BeginPlay()
{
	Super::BeginPlay();

	m_request_list.Reserve(m_members_limit_count);

	FReportUsingPointObject report_callback;
	report_callback.BindUFunction(this, FName("PermissionUsing"));

	for (AAssemblePointObject* const object : m_assembly_objects)
	{
		object->InitCallback(report_callback);
	}
}

void AAssemblyPoint::ReservateRest(ANPC_Character* const NewMember)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Reservate Rest"), *NewMember->GetUniqueName())

	if (m_request_list.Contains(NewMember)) return;

	m_request_list.Add(NewMember);
	m_observer_mgr->TriggerEvent(EObserverEventType::EOET_UsingAssembly);
}

void AAssemblyPoint::PermissionUsing(AAssemblePointObject* const PointObject)
{
	CHECK_INVALID_PTR(PointObject)
	UE_LOG(LogTemp, Warning, TEXT("%s PermissionUsing"), *PointObject->GetUniqueName())

	if (m_request_list.IsEmpty()) return;
	
	ANPC_Character* npc = PointObject->GetPointObjectUser();
	if (npc) 
	{ 
		npc->OnRestingEnd(PointObject->GetInteractionSectionName()); 
	}

	PointObject->UsePointObject(m_request_list[0]);
	m_request_list[0]->MoveToAssemblePoint(PointObject);

	m_request_list.RemoveAt(0);
}

bool AAssemblyPoint::RegisteAssemblyPointMember(ANPC_Character* const NewMember, FRequestAllowUsing& Callback)
{
	if (m_registed_members.Num() >= m_members_limit_count) return false;

	for (ANPC_Character* const member : m_registed_members)
	{
		if (member && member->GetUniqueID() == NewMember->GetUniqueID()) return false;
	}

	FRequestAllowUsing request;
	request.AddUFunction<AAssemblyPoint>(this, FName("ReservateRest"), NewMember);

	Callback = request;
	m_registed_members.Add(NewMember);

	return true;
}

void AAssemblyPoint::UnregistAssemblyPointMember(ANPC_Character* const Member)
{
	if (m_registed_members.IsEmpty() && false == m_registed_members.Contains(Member)) return;

	m_registed_members.Remove(Member);
}
