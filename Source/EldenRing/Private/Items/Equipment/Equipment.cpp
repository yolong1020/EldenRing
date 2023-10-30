// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Equipment.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items/ItemObject.h"
#include "Engine/DataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Basic/DebugMacros.h"

AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	RootComponent = m_mesh;

	m_sphere->SetupAttachment(RootComponent);
}

void AEquipment::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* item_table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/DT_ItemData.DT_ItemData'"), nullptr, LOAD_None, nullptr);
	CHECK_INVALID(item_table)

	m_item_data = item_table->FindRow<FItemData>(m_item_name, m_item_name.ToString());
	CHECK_INVALID(m_item_data)

	CHECK_INVALID(m_item_object)
	m_item_object->SetItemProperties(m_item_data, GetClass(), false);
}

void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipment::Equip(USceneComponent* parent, AActor* owner, APawn* instigator)
{
	SetOwner(owner);
	SetInstigator(instigator);

	CHECK_INVALID(m_mesh)

	FAttachmentTransformRules transform_rules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(parent, transform_rules, FName("MasterSocket"));

	UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
	CHECK_INVALID(anim_instance)
}

void AEquipment::AttachMashToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	CHECK_INVALID(m_mesh)

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

