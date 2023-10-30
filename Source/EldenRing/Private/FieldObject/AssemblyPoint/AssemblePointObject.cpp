// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/NPC/NPC_Character.h"
#include "Basic/DebugMacros.h"

AAssemblePointObject::AAssemblePointObject()
{
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = m_mesh;

	m_sphere_action = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_sphere_action->SetupAttachment(RootComponent);

	m_box_block = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Box"));
	m_box_block->SetupAttachment(RootComponent);
}

void AAssemblePointObject::BeginPlay()
{
	Super::BeginPlay();

	m_sphere_action->OnComponentBeginOverlap.AddDynamic(this, &AAssemblePointObject::OnSphereOverlap);

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	m_is_using = false;
	if (m_actor_rest)
	{
		m_is_using = true;
		m_actor_rest->InitAssemblyPointObject(this);

		m_time_using_start = world->TimeSeconds;
	}
}

void AAssemblePointObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssemblePointObject::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANPC_Character*		npc		= Cast<ANPC_Character>(OtherActor);
	USphereComponent*	sphere	= Cast<USphereComponent>(OtherComp);

	if (npc								== nullptr	||
		npc->IsPatrol()					== false	||
		npc->GetAssemblyPointObject()	!= this		||
		sphere							== nullptr	|| 
		sphere->GetFName() != FName("Interaction Sphere"))
	{ return; }

	UWorld* world = GetWorld();
	CHECK_INVALID(world)
	
	bool result = npc->InAssmeblyPointAction();
	if (result) 
	{
		m_time_using_start	= world->TimeSeconds;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Point Action"))
	}
}

void AAssemblePointObject::InitData(AAssemblyPoint* const assembly_point)
{
	m_assembly_point = assembly_point;
}

void AAssemblePointObject::ResetPointObject()
{
	m_actor_rest		= nullptr;
	m_time_using_start	= 0;
}

void AAssemblePointObject::UsePointObject(ANPC_Character* const npc)
{
	m_is_using			= true;
	m_actor_rest		= npc;
}

FVector AAssemblePointObject::GetActionLocation()
{
	if (nullptr == m_sphere_action) { return FVector::ZeroVector; }

	return m_sphere_action->GetComponentLocation();
}

const FString AAssemblePointObject::GetInteractionSectionName()
{
	switch (m_point_object_type)
	{
	case EAssemblyPointObjectType::EAPOT_Bench: return FString("Resting_Switch_Bench");
	case EAssemblyPointObjectType::EAPOT_Floor: return FString("Resting_Switch_Floor");
	default: return FString();
	}
}

