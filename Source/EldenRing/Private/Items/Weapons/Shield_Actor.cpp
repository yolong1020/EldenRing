// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Shield_Actor.h"
#include "Items/Weapons/WeaponData.h"
#include "Items/ItemObject.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameCharacter.h"
#include "Characters/Players/C0000.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Common/CommonFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "Macro/DebugMacros.h"

AShield_Actor::AShield_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShield_Actor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID_PTR(m_mesh)
	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	m_mesh->OnComponentBeginOverlap.AddDynamic(this, &AShield_Actor::OnShieldOverlap);
}

void AShield_Actor::OnShieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_is_guard || OtherActor == GetOwner()) return;

	IGameCharacter_Interface* chracter_interface = Cast<IGameCharacter_Interface>(OtherActor);
	if (!chracter_interface || chracter_interface->IsNotDamageMod()) return;

	AGameCharacter* owner = Cast<AGameCharacter>(GetOwner());
	CHECK_INVALID_PTR(owner)

	TArray<AActor*> actor_ignore;
	actor_ignore.Add(owner);

	for (AActor* actor : m_ignore_actors)
		actor_ignore.AddUnique(actor);

	if (false == owner->IsAttacking()) return;

	IHitInterface* hit_interface = Cast<IHitInterface>(OtherActor);
	if (!hit_interface) return;

	EGameDirection dir = FCommonFunctions::FindDirection(this, GetInstigator()->GetActorLocation());
	hit_interface->GetHit(dir, GetActorLocation(), m_attack_weight);

	m_ignore_actors.AddUnique(OtherActor);
	if (false == IsEnableCollision(OtherActor)) return;

	DestructableObjectAction(OtherActor->GetActorLocation());

	UGameplayStatics::ApplyDamage(OtherActor, m_value_damage + FMath::FRandRange(0, m_value_damage), GetInstigator()->GetController(), this, UDamageType::StaticClass());

	bool is_character = OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("NPC");
	if (owner->ActorHasTag("NPC") && is_character)
	{
		Cast<INPC_Interfaces>(owner)->SuccessAttack();
	}
}

void AShield_Actor::SetWeaponCollision(const ECollisionEnabled::Type& Type)
{
	CHECK_INVALID_PTR(m_mesh)
	m_mesh->SetCollisionEnabled(Type);
}
