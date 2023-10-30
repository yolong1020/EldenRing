// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_geometry_collection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(m_geometry_collection);
	m_geometry_collection->SetGenerateOverlapEvents(true);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_geometry_collection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	m_capsule->SetupAttachment(GetRootComponent());
	m_capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight)
{
	if (m_is_broken) return;

	m_is_broken = true;

	UWorld* world = GetWorld();
	if (nullptr == world || 0 >= m_treasure_classes.Num()) return;

	FVector location = GetActorLocation();
	location.Z += 1.f;

	int32 selection = FMath::RandRange(0, m_treasure_classes.Num() - 1);

	world->SpawnActor<ATreasure>(m_treasure_classes[selection], location, GetActorRotation());
}

