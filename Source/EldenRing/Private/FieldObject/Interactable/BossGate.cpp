// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Interactable/BossGate.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/Players/Player_Interface.h"
#include "System/NPCInteractionManager.h"
#include "System/ObserverManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Macro/DebugMacros.h"

ABossGate::ABossGate()
{
	PrimaryActorTick.bCanEverTick = true;

	m_blind_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Blind"));
	m_blind_particle->SetupAttachment(m_mesh);

	m_sphere_detect = CreateDefaultSubobject<USphereComponent>(FName("Detect Sphere"));
	CHECK_INVALID_PTR(m_sphere_detect)
	m_sphere_detect->SetupAttachment(RootComponent);
	m_sphere_detect->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_sphere_detect->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_sphere_detect->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	m_sphere_detect->SetGenerateOverlapEvents(true);

	m_box_block = CreateDefaultSubobject<UBoxComponent>(FName("Block Box"));
	CHECK_INVALID_PTR(m_box_block)
	m_box_block->SetupAttachment(RootComponent);
	m_box_block->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_box_block->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_box_block->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	m_sphere_interaction->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorTickInterval(1);
}

void ABossGate::DoFirstInteract()
{
	m_box_block->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_box_interaction->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FLatentActionInfo callback;
	callback.CallbackTarget		= this;
	callback.ExecutionFunction	= FName("ToggleBlock");
	callback.Linkage			= 0;
	callback.UUID				= FGuid::NewGuid().A;

	UKismetSystemLibrary::Delay(GetWorld(), 3, callback);
}

void ABossGate::BeginPlay()
{
	Super::BeginPlay();

	m_box_interaction->OnComponentBeginOverlap.AddDynamic(this, &ABossGate::InInteractOverlap);
	m_box_interaction->OnComponentEndOverlap.AddDynamic(this, &ABossGate::OutInteractOverlap);

	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_observer_mgr->RegistObserver<ABossGate>(EObserverEventType::EOET_BossGate, this, &ABossGate::ResetBlock);
}

void ABossGate::Tick(float DeltaSeconds)
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors, ANPC_Character::StaticClass());

	m_is_detacted_enemy = false;
	for (int32 i = 0; i < actors.Num(); ++i)
	{
		AGameCharacter* character = Cast<AGameCharacter>(actors[i]);
		if (!character) continue;

		if (character->ActorHasTag("Monster") && character->IsAlive())
		{
			m_is_detacted_enemy = true;

			CHECK_INVALID_SMART_PTR(m_interact_mgr)
			if (m_interact_mgr->CheckSameInteractPopUp(EInteractPopupType::EIPT_ToBonefire))
			{
				m_interact_mgr->CloseInteractPopUp();
			} break;
		}
	}
}

void ABossGate::InInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")) && false == m_is_detacted_enemy )
	{
		IPlayer_Interface* player_interface = Cast<IPlayer_Interface>(OtherActor);
		CHECK_INVALID_PTR(player_interface)

		CHECK_INVALID_SMART_PTR(m_interact_mgr)
		m_interact_mgr->SetInterface(this);
		m_interact_mgr->OpenInteractPopUp(player_interface->GetControllerComponent(), EInteractPopupType::EIPT_ToBossArea);
	}
}

void ABossGate::OutInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")) && false == m_is_detacted_enemy)
	{
		CHECK_INVALID_SMART_PTR(m_interact_mgr)
		m_interact_mgr->CloseInteractPopUp();
	}
}

void ABossGate::ToggleImpactFX()
{
	//m_blind_particle->ToggleActive();
}

void ABossGate::ToggleBlock()
{
	CHECK_INVALID_PTR(m_box_block)

	ECollisionEnabled::Type type = m_box_block->GetCollisionEnabled();
	m_box_block->SetCollisionEnabled(type == ECollisionEnabled::NoCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void ABossGate::ResetBlock()
{
	m_box_block->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_box_interaction->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
