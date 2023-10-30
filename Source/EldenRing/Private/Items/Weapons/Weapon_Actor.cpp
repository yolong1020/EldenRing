// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/WeaponData.h"
#include "Items/ItemObject.h"
#include "Characters/Players/C0000.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Characters/GameCharacter.h"
#include "Characters/Players/C0000.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Engine/DataTable.h"
#include "NiagaraComponent.h"
#include "Basic/DebugMacros.h"

AWeapon_Actor::AWeapon_Actor()
{
 	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	RootComponent = m_mesh;

	m_sphere->SetupAttachment(RootComponent);

	m_field_system = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field System"));

	CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Field System Meta Data Filter"));

	FString	    row_name	 = StaticEnum<EWeaponType>()->GetNameStringByValue(int64(m_weapon_type));
	UDataTable* weapon_table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/DT_WeaponData.DT_WeaponData'"), nullptr, LOAD_None, nullptr);

	CHECK_INVALID(weapon_table)
	FWeaponData* row_data = weapon_table->FindRow<FWeaponData>(FName(row_name), row_name);

	CHECK_INVALID(row_data) 
	m_weapon_data = row_data;
}

int32 AWeapon_Actor::GetWeaponDamage(const EAttackType& attack_type, const EAttackStrength& attack_strength)
{
	if (attack_type == EAttackType::EATKT_ParryAttack) return m_value_damage * FMath::FRandRange(8, 12);
	else
	{
		int32 damage_range = FMath::FRandRange(0, m_value_damage);
		switch (attack_type)
		{
			case EAttackType::EATKT_Attack:	     return m_value_damage + damage_range;
			case EAttackType::EATKT_DashAttack:  return m_value_damage + (damage_range * 1.5);
			case EAttackType::EATKT_JumpAttack:  return m_value_damage + (damage_range * 1.2);
		}
	}

	return 0;
}

void AWeapon_Actor::BeginPlay()
{
	Super::BeginPlay();
	
	m_is_using = false;
	
	UDataTable* item_table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/DT_ItemData.DT_ItemData'"), nullptr, LOAD_None, nullptr);
	CHECK_INVALID(item_table)

	m_item_data = item_table->FindRow<FItemData>(m_item_name, m_item_name.ToString());
	CHECK_INVALID(m_item_data)

	CHECK_INVALID(m_item_object)
	m_item_object->SetItemProperties(m_item_data, GetClass(), false);
}

void AWeapon_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_Actor::DestructableObjectAction(const FVector& field_location)
{
	UObject* falloff	= GetDefaultSubobjectByName("Radial Falloff");
	UObject* vector		= GetDefaultSubobjectByName("Radial Vector");
	UObject* data_filter	= GetDefaultSubobjectByName("Field System Meta Data Filter");

	URadialFalloff*	radial_falloff	 = (nullptr != falloff)	? Cast<URadialFalloff>(falloff) : nullptr;
	URadialVector*	radial_vector	 = (nullptr != vector)	? Cast<URadialVector>(vector) : nullptr;
	UFieldSystemMetaDataFilter* meta_data_filter = (nullptr != data_filter)	? Cast<UFieldSystemMetaDataFilter>(data_filter) : nullptr;

	if (nullptr == radial_falloff	||
	    nullptr == radial_vector	||
	    nullptr == meta_data_filter ||
	    nullptr == m_field_system) return;

	radial_falloff->SetRadialFalloff(
		m_radial_falloff_magnitude,
		m_radial_falloff_range_min,
		m_radial_falloff_range_max,
		0,
		m_radial_falloff_radius,
		field_location,
		m_radial_falloff_type);

	radial_vector->SetRadialVector(m_radial_vector_magnitude, field_location);

	meta_data_filter->SetMetaDataFilterType(
		EFieldFilterType::Field_Filter_Dynamic,
		EFieldObjectType::Field_Object_Destruction,
		EFieldPositionType::Field_Position_CenterOfMass);

	m_field_system->ApplyPhysicsField(
		true,
		EFieldPhysicsType::Field_ExternalClusterStrain,
		nullptr,
		radial_falloff);

	m_field_system->ApplyPhysicsField(
		true,
		EFieldPhysicsType::Field_LinearForce,
		meta_data_filter,
		radial_vector);
}

bool AWeapon_Actor::IsEnableCollision(AActor* const OtherActor)
{
	if (nullptr == OtherActor) return false;

	AActor* owner = GetOwner();
	if (nullptr == owner || owner == OtherActor) return false;
	
	AC0000* player = Cast<AC0000>(OtherActor);
	if (nullptr != player && player->IsNotDamageMod()) return false;

	if (owner->ActorHasTag("NPC") && OtherActor->ActorHasTag("NPC"))
	{
		if (owner->GetClass() == OtherActor->GetClass()) return false;
	}

	return true;
}

void AWeapon_Actor::Equip(USceneComponent* parent, FName socket_name, AActor* owner, APawn* instigator)
{
	SetOwner(owner);
	SetInstigator(instigator);

	if (nullptr == m_mesh) return;

	FAttachmentTransformRules transform_rules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(parent, transform_rules, socket_name);
	SetSphereCollisionActivation(ECollisionEnabled::NoCollision);
}

void AWeapon_Actor::SetAttackWeight(const EEquipState& equip_state, const EAttackStrength& attack_strength, const EAttackType& attack_type)
{
	TArray<EAttackWeight> attack_weight;

	switch (attack_type)
	{
	case EAttackType::EATKT_Attack:	    attack_weight = m_weapon_data->Attack;	break;
	case EAttackType::EATKT_DashAttack: attack_weight = m_weapon_data->DashAttack;	break;
	case EAttackType::EATKT_JumpAttack: attack_weight = m_weapon_data->JumpAttack;	break;
	default: return;
	}

	if ((int32)attack_strength > attack_weight.Num()) return;

	m_attack_weight = attack_weight[(int32)attack_strength - 1];
}

void AWeapon_Actor::AttachMashToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	if (nullptr == m_mesh) return;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}
