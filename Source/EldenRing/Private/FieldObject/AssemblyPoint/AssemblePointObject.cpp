// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/NPC/NPC_Character.h"
#include "System/ObserverManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Macro/DebugMacros.h"

AAssemblePointObject::AAssemblePointObject() : m_is_using(false), m_is_reserved(false)
{
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = m_mesh;

	m_sphere_action = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_sphere_action->SetupAttachment(RootComponent);

	m_box_block = CreateDefaultSubobject<UBoxComponent>(TEXT("Block Box"));
	m_box_block->SetupAttachment(RootComponent);

	m_latent.CallbackTarget = this;
	m_latent.Linkage = 0;
	m_latent.ExecutionFunction = FName("ReportUsingTime");
}

void AAssemblePointObject::BeginPlay()
{
	Super::BeginPlay();

	m_sphere_action->OnComponentBeginOverlap.AddDynamic(this, &AAssemblePointObject::OnSphereOverlap);

	m_observer_mgr->RegistObserver<AAssemblePointObject>(EObserverEventType::EOET_UsingAssembly, this, &AAssemblePointObject::ReportUsingTime);

	if (m_actor_rest)
	{
		m_actor_rest->InitAssemblyPointObject(this);
		m_time_using_start = GetWorld()->TimeSeconds;
		m_is_using = true;
	}
}

void AAssemblePointObject::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANPC_Character*	  npc	 = Cast<ANPC_Character>(OtherActor);
	USphereComponent* sphere = Cast<USphereComponent>(OtherComp);

	if (!npc || !sphere || sphere->GetFName() != FName("Interaction Sphere")) return;

	bool result = npc->InAssmeblyPointAction();
	if (result)
	{
		m_time_using_start = GetWorld()->TimeSeconds;
		m_is_using	   = true;
		m_is_reserved	   = false;
		m_sphere_action->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AAssemblePointObject::ReportUsingTime()
{
	if (false == m_is_using && false == m_is_reserved)
	{
		m_report_callback.Execute(this);
		return;
	}
	
	double total_using = GetWorld()->TimeSeconds - m_time_using_start;
	if (false == m_is_reserved && total_using >= m_time_limit)
	{
		m_report_callback.Execute(this);
		return;
	}

	m_latent.UUID     = FGuid::NewGuid().A;
	double delay_time = abs(m_time_limit - total_using);
	UKismetSystemLibrary::Delay(GetWorld(), delay_time, m_latent);
}

void AAssemblePointObject::UsePointObject(ANPC_Character* const NPC)
{
	m_actor_rest		= NPC;
	m_time_using_start	= GetWorld()->TimeSeconds;
	m_is_reserved		= true;
	m_sphere_action->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

FVector AAssemblePointObject::GetActionLocation()
{
	return nullptr == m_sphere_action ? FVector::ZeroVector : m_sphere_action->GetComponentLocation();
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

void AAssemblePointObject::InitCallback(const FReportUsingPointObject& Callback)
{
	m_report_callback = Callback;
}

void AAssemblePointObject::ResetPointObject()
{
	m_time_using_start	= 0;
	m_is_using			= false;
	m_is_reserved		= false;
	m_sphere_action->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

const bool AAssemblePointObject::UseAgainPointObject()
{
	if (m_is_using)
	{
		m_sphere_action->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		return true;
	}
	else return false;
}

