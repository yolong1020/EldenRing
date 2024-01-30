// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Equipment.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items/ItemObject.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/DataTableManager.h"
#include "Macro/DebugMacros.h"

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

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	UDataTableManager* datatable_mgr = instance->GetSubsystem<UDataTableManager>();
	CHECK_INVALID_PTR(datatable_mgr)

	UDataTable* item_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Item);
	CHECK_INVALID_PTR(item_table)

	FItemData* data = item_table->FindRow<FItemData>(m_item_name, m_item_name.ToString());
	CHECK_INVALID_PTR(data)

	m_item_data = *data;
	m_item_object->SetItemProperties(m_item_name, m_item_data, GetClass(), false);
}

void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipment::Equip(USceneComponent* parent, AActor* owner, APawn* instigator)
{
	SetOwner(owner);
	SetInstigator(instigator);

	CHECK_INVALID_PTR(m_mesh)

	FAttachmentTransformRules transform_rules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(parent, transform_rules, FName("MasterSocket"));

	UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
	CHECK_INVALID_PTR(anim_instance)
}

void AEquipment::AttachMashToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	CHECK_INVALID_PTR(m_mesh)

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	m_mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

