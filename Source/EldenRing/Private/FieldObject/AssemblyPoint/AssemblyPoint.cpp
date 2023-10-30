// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CascadeParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/NPC/NPC_Character.h"
#include "Basic/DebugMacros.h"

AAssemblyPoint::AAssemblyPoint()
{
 	PrimaryActorTick.bCanEverTick = true;

	m_mesh			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent	= m_mesh;

	m_fire_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particle Component"));
	m_fire_particle->SetupAttachment(RootComponent);

	m_smoke_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Particle Component"));
	m_smoke_particle->SetupAttachment(RootComponent);

	m_sphere_block = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_sphere_block->SetupAttachment(RootComponent);
}

void AAssemblyPoint::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID(m_fire_sound)
	UGameplayStatics::PlaySoundAtLocation(this, m_fire_sound, GetActorLocation());

	for (AAssemblePointObject* const object : m_assembly_objects)
	{
		object->InitData(this);
	}
}

bool AAssemblyPoint::RegisterAssemblyPointMember(ANPC_Character* const new_member)
{
	if (m_registed_members.Num() >= m_members_limit_count) { return false; }

	for (ANPC_Character* const member : m_registed_members)
	{
		if (member->GetUniqueID() == new_member->GetUniqueID()) { return false; }
	}

	m_registed_members.Add(new_member);

	return true;
}

AAssemblePointObject* const AAssemblyPoint::TryUsePointObject(ANPC_Character* const request_npc)
{
	for (AAssemblePointObject* const object : m_assembly_objects)
	{
		if (false == object->IsUsing()) { return object; }
	}

	UWorld* world = GetWorld();
	if (nullptr == world) { return nullptr; }

	double time_current = world->TimeSeconds;

	Algo::Sort(m_assembly_objects, [time_current](AAssemblePointObject* left, AAssemblePointObject* right)
		{
			return (time_current - left->GetTimeStartUsing()) > (time_current - right->GetTimeStartUsing());
		});

	int32	index			= 0;
	bool	isDoneRestEnd	= false;

	for (int i = 0; i < m_assembly_objects.Num(); ++i)
	{
		ANPC_Character* npc = m_assembly_objects[i]->GetPointObjectUser();
		if (nullptr == npc) { return nullptr; }

		if (false == npc->IsPatrol())
		{
			index = i;
			if (false == m_assembly_objects.IsValidIndex(index)) { return nullptr; }

			UE_LOG(LogTemp, Warning, TEXT("On Resting End!"))
			npc->OnRestingEnd(m_assembly_objects[index]->GetInteractionSectionName());
			isDoneRestEnd = true;
			break;
		}
	}

	m_assembly_objects[index]->ResetPointObject();
	m_assembly_objects[index]->UsePointObject(request_npc);

	return m_assembly_objects[index];
}

void AAssemblyPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

