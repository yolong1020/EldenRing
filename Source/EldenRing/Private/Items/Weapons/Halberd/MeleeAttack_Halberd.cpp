// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Halberd/MeleeAttack_Halberd.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/Players/C0000.h"
#include "Items/Weapons/Shield_Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Macro/DebugMacros.h"

AMeleeAttack_Halberd::AMeleeAttack_Halberd()
{
	PrimaryActorTick.bCanEverTick = true;

	m_weapon_box_extra = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Extra Box"));
	m_weapon_box_extra->SetupAttachment(RootComponent);

	m_boxtrace_start_extra = CreateDefaultSubobject<USceneComponent>(TEXT("Extra Box Trace Start"));
	m_boxtrace_start_extra->SetupAttachment(RootComponent);
	m_boxtrace_end_extra = CreateDefaultSubobject<USceneComponent>(TEXT("Extra Box Trace End"));
	m_boxtrace_end_extra->SetupAttachment(RootComponent);
}

void AMeleeAttack_Halberd::SetWeaponCollision(const ECollisionEnabled::Type& Type)
{
	CHECK_INVALID_PTR(m_weapon_box)
	m_weapon_box->SetCollisionEnabled(Type);

	CHECK_INVALID_PTR(m_weapon_box_extra)
	m_weapon_box_extra->SetCollisionEnabled(Type);

	if (Type == ECollisionEnabled::NoCollision) { m_ignore_actors.Empty(); }
}

void AMeleeAttack_Halberd::BeginPlay()
{
	Super::BeginPlay();
}
