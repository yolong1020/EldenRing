// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/MeleeAttackBody_Actor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Hit/HitInterface.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/GameCharacter_Interface.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Common/CommonFunctions.h"
#include "Macro/DebugMacros.h"

AMeleeAttackBody_Actor::AMeleeAttackBody_Actor()
{
	PrimaryActorTick.bCanEverTick = true;

	m_weapon_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Weapon Capsule"));
	CHECK_INVALID_PTR(m_weapon_capsule)
	RootComponent = m_weapon_capsule;

	m_field_system = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field System"));

	CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Field System Meta Data Filter"));
}

void AMeleeAttackBody_Actor::InitAttackBodyCollision(USceneComponent* const Parent, const FName& SocketName, const FVector& Location, const FRotator& Rotator, const float& Radius, const float& Height, AActor* OwnerActor, APawn* InstigatorPawn)
{
	SetOwner(OwnerActor);
	SetInstigator(InstigatorPawn);

	CHECK_INVALID_PTR (m_weapon_capsule)
	FAttachmentTransformRules transform_rules(EAttachmentRule::SnapToTarget, true);
	m_weapon_capsule->AttachToComponent(Parent, transform_rules, SocketName);
	m_weapon_capsule->SetRelativeLocationAndRotation(Location, Rotator.Quaternion());
	m_weapon_capsule->SetCapsuleSize(Radius, Height);
}

void AMeleeAttackBody_Actor::SetWeaponCollision(const ECollisionEnabled::Type& Type)
{
	CHECK_INVALID_PTR (m_weapon_capsule) 
	m_weapon_capsule->SetCollisionEnabled(Type);
}

void AMeleeAttackBody_Actor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID_PTR(m_weapon_capsule)
	m_weapon_capsule->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAttackBody_Actor::OnCapsuleOverlap);
	m_weapon_capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_weapon_capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_weapon_capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_weapon_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	m_weapon_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_weapon_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AMeleeAttackBody_Actor::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == IsEnableCollision(OtherActor)) return;

	AActor* owner = GetOwner();
	CHECK_INVALID_PTR(owner)

	IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(owner);
	CHECK_INVALID_PTR(character_interface)
	
	TArray<AActor*> actor_ignore;
	actor_ignore.Add(this);
	for (AActor* actor : character_interface->GetIgnoreTargets())
	{
		actor_ignore.AddUnique(actor);
	}

	UCapsuleComponent* trigger = Cast<UCapsuleComponent>(OverlappedComponent);
	CHECK_INVALID_PTR(trigger)

	float	height	 = trigger->GetScaledCapsuleHalfHeight();
	FVector location = trigger->GetComponentLocation();
	FVector start	 = FVector(location.X, location.Y, location.Z - height);
	FVector end		 = FVector(location.X, location.Y, location.Z + height);

	FHitResult hit;
	bool hit_result = UKismetSystemLibrary::CapsuleTraceSingle(this,
		start, end,
		trigger->GetScaledCapsuleRadius(),
		height,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		actor_ignore,
		EDrawDebugTrace::None,
		hit,
		true);

	AActor* target = hit.GetActor();
	if (nullptr != target)
	{
		IHitInterface* interface = Cast<IHitInterface>(target);
		if (!interface) return;

		character_interface->AddIgnoreTargets(target);
		if (false == IsEnableCollision(target)) return;

		//DestructableObjectAction(target->GetActorLocation());

		UGameplayStatics::ApplyDamage(target, 200, GetInstigator()->GetController(), this, UDamageType::StaticClass());

		AActor* instigator = GetInstigator();
		CHECK_INVALID_PTR(instigator)

		EGameDirection dir = FCommonFunctions::FindDirection(target, instigator->GetActorLocation());
		interface->GetHit(dir, hit.ImpactPoint, EAttackWeight::EAW_UltraHeavy);

		bool is_character = target->ActorHasTag("Player") || target->ActorHasTag("NPC");
		bool is_npc		  = ActorHasTag("NPC");
		INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(owner);
		if (npc_interface && is_npc && is_character)
		{
			npc_interface->SuccessAttack();
		}
	}
}

bool AMeleeAttackBody_Actor::IsEnableCollision(AActor* const OtherActor)
{
	if (nullptr == OtherActor) return false;

	AActor* owner = GetOwner();
	if (nullptr == owner || owner == OtherActor) return false;

	IGameCharacter_Interface* character = Cast<IGameCharacter_Interface>(OtherActor);
	if (nullptr != character && character->IsNotDamageMod()) return false;

	if (owner->ActorHasTag("NPC") && OtherActor->ActorHasTag("NPC"))
	{
		if (owner->GetClass() == OtherActor->GetClass()) return false;
	}

	return true;
}
