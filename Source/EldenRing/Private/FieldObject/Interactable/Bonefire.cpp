// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Interactable/Bonefire.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "System/NPCInteractionManager.h"
#include "System/ObserverManager.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "Characters/GameCharacter.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/Players/Player_Interface.h"
#include "GameBase/ERGameInstance.h"
#include "Macro/DebugMacros.h"

ABonefire::ABonefire()
{
	PrimaryActorTick.bCanEverTick = true;

	m_ember_particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX_Ember"));
	m_ember_particle->SetupAttachment(m_mesh);

	m_crystal_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Crystal"));
	m_crystal_particle->SetupAttachment(m_mesh);

	m_halo_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Halo"));
	m_halo_particle->SetupAttachment(m_mesh);

	m_reflection_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Reflection"));
	m_reflection_particle->SetupAttachment(m_mesh);

	m_impact_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Impact"));
	m_impact_particle->SetupAttachment(m_mesh);

	m_sphere_detect = CreateDefaultSubobject<USphereComponent>(FName("Detect Sphere"));
	CHECK_INVALID_PTR(m_sphere_detect)
	m_sphere_detect->SetupAttachment(RootComponent);

	m_sphere_detect->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_sphere_detect->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_sphere_detect->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	m_sphere_detect->SetGenerateOverlapEvents(true);

	SetActorTickInterval(1);
}

void ABonefire::DoFirstInteract()
{
	m_is_first = false;

	FLatentActionInfo callback;
	callback.UUID			= FGuid::NewGuid().A;
	callback.CallbackTarget = this;
	callback.Linkage		= 0;
	callback.ExecutionFunction = FName("ToggleImpactFX");

	UKismetSystemLibrary::Delay(GetWorld(), 1.4f, callback);

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_PTR(observer_mgr)
	observer_mgr->TriggerEvent(EObserverEventType::EOET_GraceDiscovered);
	
	instance->ResetSaveData();
	observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
}

void ABonefire::BeginPlay()
{
	Super::BeginPlay();

	m_sphere_interaction->OnComponentBeginOverlap.AddDynamic(this, &ABonefire::InInteractOverlap);
	m_sphere_interaction->OnComponentEndOverlap.AddDynamic(this, &ABonefire::OutInteractOverlap);
}

void ABonefire::Tick(float DeltaSeconds)
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors, ANPC_Character::StaticClass());

	m_is_detacted_enemy = false;
	for (AActor* const actor : actors)
	{
		AGameCharacter* character = Cast<AGameCharacter>(actor);
		if (!character) continue;

		bool is_alive = character->IsAlive();
		if (character->ActorHasTag("Monster") && is_alive)
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

void ABonefire::InInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPlayer_Interface* player_interface = Cast<IPlayer_Interface>(OtherActor);
	if (!player_interface) return;

	if (false == m_is_detacted_enemy)
	{
		CHECK_INVALID_SMART_PTR(m_interact_mgr)
		m_interact_mgr->SetInterface(this);
		m_interact_mgr->OpenInteractPopUp(player_interface->GetControllerComponent(), EInteractPopupType::EIPT_ToBonefire, m_is_first);
	}
}

void ABonefire::OutInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")) && false == m_is_detacted_enemy)
	{
		CHECK_INVALID_SMART_PTR(m_interact_mgr)
		m_interact_mgr->CloseInteractPopUp();
	}
}

void ABonefire::ToggleImpactFX()
{
	m_impact_particle->ToggleActive();
}
