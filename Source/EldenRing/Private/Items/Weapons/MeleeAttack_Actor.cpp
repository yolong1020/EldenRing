// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/Players/C0000.h"
#include "Items/Weapons/Shield_Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Common/CommonFunctions.h"
#include "NiagaraComponent.h"
#include "FieldObject/Breakable/BreakableActor.h"
#include "Macro/DebugMacros.h"

AMeleeAttack_Actor::AMeleeAttack_Actor()
{
	PrimaryActorTick.bCanEverTick = true;

	m_weapon_box = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	m_weapon_box->SetupAttachment(RootComponent);

	m_boxtrace_start = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	m_boxtrace_start->SetupAttachment(RootComponent);
	m_boxtrace_end = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	m_boxtrace_end->SetupAttachment(RootComponent);

	m_effect_sparks = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sparks"));
	m_effect_sparks->SetupAttachment(RootComponent);
}

void AMeleeAttack_Actor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID_PTR(m_mesh)
	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CHECK_INVALID_PTR(m_weapon_box)
	m_weapon_box->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAttack_Actor::OnBoxOverlap);
	m_weapon_box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_weapon_box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_weapon_box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_weapon_box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	m_weapon_box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_weapon_box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	if (m_weapon_box_extra)
	{
		m_weapon_box_extra->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAttack_Actor::OnBoxOverlap);
		m_weapon_box_extra->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_weapon_box_extra->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		m_weapon_box_extra->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		m_weapon_box_extra->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		m_weapon_box_extra->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		m_weapon_box_extra->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

	if (nullptr != m_effect_sparks) { m_effect_sparks->SetActive(false); }
}

void AMeleeAttack_Actor::SetWeaponCollision(const ECollisionEnabled::Type& Type)
{
	CHECK_INVALID_PTR(m_weapon_box)
	m_weapon_box->SetCollisionEnabled(Type);

	if (Type == ECollisionEnabled::NoCollision) { m_ignore_actors.Empty(); }
}

void AMeleeAttack_Actor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == IsEnableCollision(OtherActor)) return;

	AActor* owner = GetOwner();
	CHECK_INVALID_PTR(owner)

	USphereComponent* component = Cast<USphereComponent>(OtherComp);
	if (component)
	{
		AGameCharacter* defender = Cast<AGameCharacter>(OtherActor);
		if (defender && component == defender->GetParrySphere())
		{
			AGameCharacter* attacker = Cast<AGameCharacter>(owner);
			if (attacker)
			{
				attacker->OnTakeStunn();
				m_ignore_actors.AddUnique(defender);
				return;
			}
		}
	}

	AShield_Actor* shield = Cast<AShield_Actor>(OtherActor);
	if (shield)
	{
		AGameCharacter* defender = Cast<AGameCharacter>(shield->GetOwner());
		AGameCharacter* attacker = Cast<AGameCharacter>(owner);
		CHECK_INVALID_PTR(defender)
		CHECK_INVALID_PTR(attacker)

		if (defender->GetClass() == attacker->GetClass()) return;

		if (attacker->IsCurrentActionState(FString("Attacking")) &&
			defender->IsGuardState())
		{
			m_ignore_actors.AddUnique(defender);

			defender->OnAttackDefended(m_attack_weight);
			attacker->OnAttackBlocked(m_attack_weight);

			if (m_weapon_sound)  { UGameplayStatics::PlaySoundAtLocation(this, m_weapon_sound, GetActorLocation()); }
			if (m_effect_sparks) { m_effect_sparks->SetActive(true); }
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail Guard"))
		}
	}
	else
	{
		const FVector start = m_boxtrace_start->GetComponentLocation();
		const FVector end   = m_boxtrace_end->GetComponentLocation();

		TArray<AActor*> actor_ignore;
		actor_ignore.Add(this);
		actor_ignore.Add(owner);

		for (AActor* actor : m_ignore_actors)
			actor_ignore.AddUnique(actor);

		FHitResult hit;
		bool hit_result = UKismetSystemLibrary::BoxTraceSingle(this,
			start, end,
			FVector(5, 5, 5),
			m_boxtrace_start->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			actor_ignore,
			EDrawDebugTrace::None,
			hit,
			true
		);

		AActor* target = hit.GetActor();
		if (target)
		{
			IHitInterface* hit_interface = Cast<IHitInterface>(target);
			if (!hit_interface) return;

			m_ignore_actors.AddUnique(target);
			if (false == IsEnableCollision(target)) return;

			DestructableObjectAction(target->GetActorLocation());

			UGameplayStatics::ApplyDamage(target, m_value_damage + FMath::FRandRange(0, m_value_damage * 0.5), GetInstigator()->GetController(), this, UDamageType::StaticClass());
			EGameDirection dir = FCommonFunctions::FindDirection(target, GetInstigator()->GetActorLocation());
			hit_interface->GetHit(dir, hit.ImpactPoint, m_attack_weight);

			bool is_character = target->ActorHasTag("Player") || target->ActorHasTag("NPC");
			if (owner->ActorHasTag("NPC") && is_character)
			{
				Cast<ANPC_Character>(owner)->SuccessAttack();
			}
		}
	}
}
