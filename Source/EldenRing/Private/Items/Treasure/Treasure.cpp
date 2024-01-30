// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure/Treasure.h"
#include "Characters/Players/C0000.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Macro/DebugMacros.h"

ATreasure::ATreasure()
{
	m_mesh			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent	= m_mesh;

	m_sphere->SetupAttachment(RootComponent);

	m_effect_embers = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	m_effect_embers->SetupAttachment(RootComponent);
}

void ATreasure::InitDroppedTreasureFromPlayer(const int32& amount)
{
	m_is_dropped_from_player = true;
	m_value = amount;
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC0000* player = Cast<AC0000>(OtherActor);
	if (!player) return;

	int32 gold = m_is_dropped_from_player ? m_value : (int32)FMath::RandRange(m_value_min, m_value_max);
	player->SetGold(gold);

	CHECK_INVALID_PTR(m_sound_pickup)
	UGameplayStatics::PlaySoundAtLocation(
		this,
		m_sound_pickup,
		GetActorLocation()
	);

	Destroy();
}
