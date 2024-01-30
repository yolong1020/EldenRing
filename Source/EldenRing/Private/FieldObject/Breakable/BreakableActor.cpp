// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Breakable/BreakableActor.h"
#include "System/ObserverManager.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Macro/DebugMacros.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	m_geometry_collection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(m_geometry_collection);

	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	m_capsule->SetupAttachment(GetRootComponent());
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID_PTR(m_geometry_collection)
	m_geometry_collection->SetGenerateOverlapEvents(true);
	m_geometry_collection->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	m_geometry_collection->SetSimulatePhysics(false);

	CHECK_INVALID_PTR(m_capsule)
	m_capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	m_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	m_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void ABreakableActor::GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight)
{
	if (m_is_broken) return;
	m_is_broken = true;

	m_geometry_collection->SetSimulatePhysics(true);
	m_capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(3);

	if (m_sound_hit) { UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, GetActorLocation()); }

	UWorld* world = GetWorld();
	if (!world || 0 >= m_treasure_classes.Num()) return;

	FVector location = GetActorLocation();
	location.Z += 1.f;
	int32 selection = FMath::RandRange(0, m_treasure_classes.Num() - 1);
	world->SpawnActor<ATreasure>(m_treasure_classes[selection], location, GetActorRotation());
}

