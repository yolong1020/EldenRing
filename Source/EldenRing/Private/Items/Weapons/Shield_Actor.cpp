// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Shield_Actor.h"
#include "Items/Weapons/WeaponData.h"
#include "Items/ItemObject.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameCharacter.h"
#include "Characters/Players/C0000.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "NiagaraComponent.h"
#include "Basic/DebugMacros.h"

AShield_Actor::AShield_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShield_Actor::BeginPlay()
{
	Super::BeginPlay();
}

void AShield_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShield_Actor::SetShieldCollision(const ECollisionEnabled::Type& type)
{
	CHECK_INVALID(m_mesh)
	m_mesh->SetCollisionEnabled(type);
}
