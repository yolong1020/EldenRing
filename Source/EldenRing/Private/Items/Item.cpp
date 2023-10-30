// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Items/ItemObject.h"
#include "Components/SphereComponent.h"
#include "Characters/Players/SlashCharacter.h"
#include "Characters/Players/C0000.h"
#include "Items/ItemData.h"
#include "NiagaraComponent.h"
#include "Basic/Basic.h"
#include "Basic/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	m_item_object	= CreateDefaultSubobject<UItemObject>(TEXT("Item Object"));
	m_sphere		= CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	m_sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	m_sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::SetSphereCollisionActivation(ECollisionEnabled::Type type)
{
	CHECK_INVALID(m_sphere)
	m_sphere->SetCollisionEnabled(type);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC0000* c0000 = Cast<AC0000>(OtherActor);
	CHECK_INVALID(c0000)

	c0000->SetOverlappingItem(this);
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AC0000* c0000 = Cast<AC0000>(OtherActor);
	CHECK_INVALID(c0000)

	c0000->SetOverlappingItem(nullptr);
}


