// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Vender/Vender.h"
#include "GroomComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/Component/AttributeComponent.h"
#include "Characters/Players/C0000.h"
#include "Camera/CameraComponent.h"
#include "Camera/DirectCamera.h"
#include "Items/ItemData.h"
#include "Items/ItemObject.h"
#include "Items/ItemVendor.h"
#include "Items/Equipment/Equipment.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "System/DataTableManager.h"
#include "System/NPCInteractionManager.h"
#include "System/ObserverManager.h"
#include "System/Event/EventTypes.h"
#include "System/Event/EventDelegates.h"
#include "System/TradeManager.h"
#include "System/ObserverManager.h"
#include "UI/PopUp/PopUpTypes.h"
#include "Characters/Players/Player_Interface.h"
#include "GameBase/ERGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FieldObject/Interactable/OptionalTalkboxPoint.h"
#include "Macro/DebugMacros.h"

AVender::AVender() : m_communicate_idx(0)
{
	PrimaryActorTick.bCanEverTick = false;

	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_execution_back->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVender::OnMoveCompleted(const FPathFollowingResult& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveCompleted"))
}

void AVender::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Echo"));
	Tags.Add(FName("Vender"));
	Tags.Add(FName("InteractObject"));

	m_movement_component = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	CHECK_INVALID_SMART_PTR(m_movement_component)

	m_movement_component->bOrientRotationToMovement = true;
	m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;
	m_movement_component->RotationRate = FRotator(0.f, 400.f, 0.f);

	CHECK_INVALID_PTR(m_sphere_interaction)
	m_sphere_interaction->OnComponentBeginOverlap.AddDynamic(this, &AVender::OnInteractOverlap);
	m_sphere_interaction->OnComponentEndOverlap.AddDynamic(this, &AVender::OnInteractEnd);
	m_sphere_interaction->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_sphere_interaction->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_sphere_interaction->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_sphere_interaction->SetGenerateOverlapEvents(true);

	UERGameInstance* instance = Cast<UERGameInstance>(UGameplayStatics::GetGameInstance(this));
	CHECK_INVALID_PTR(instance)

	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_PTR(observer_mgr)
	observer_mgr->RegistObserver<AVender>(EObserverEventType::EOET_GraceDiscovered, this, &AVender::OnCompleteGraceDiscover);
	observer_mgr->RegistObserver<AVender>(EObserverEventType::EOET_SaveData, this, &AVender::SaveCharacterData);
	observer_mgr->RegistObserver<AVender>(EObserverEventType::EOET_SaveData, this, &AVender::SaveInteractObjectData);

	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_SMART_PTR(m_trade_mgr)

	m_dialouge_table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/Dialouge/DT_EchoDialouge.DT_EchoDialouge'"), nullptr, LOAD_None, nullptr);
	CHECK_INVALID_PTR(m_dialouge_table)

	if (instance->IsNewGame())
	{
		UDataTableManager* datatable_mgr = instance->GetSubsystem<UDataTableManager>();
		CHECK_INVALID_PTR(datatable_mgr)
		UDataTable* vendor_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Vender);
		CHECK_INVALID_PTR(vendor_table)
		FItemVendor* item_list = vendor_table->FindRow<FItemVendor>(FName(m_name), m_name);
		CHECK_INVALID_PTR(item_list)
		UDataTable* item_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Item);
		CHECK_INVALID_PTR(item_table)

		for (const FName& name : item_list->ItemList)
		{
			FItemData* data = item_table->FindRow<FItemData>(name, name.ToString());
			if (data)
			{
				UClass* type = nullptr;
				FString path = "WidgetBlueprint \'" + data->BluePrintPath + "\'";

				if (data->EquipmentType == EEquipmentType::EET_Weapon) { type = LoadClass<AWeapon_Actor>(nullptr, *path); }
				else						       { type = LoadClass<AEquipment>(nullptr, *path); }

				TObjectPtr<UItemObject> obj = NewObject<UItemObject>();
				obj->SetItemProperties(name, *data, type, false);
				obj->SetCurrentHUD(EDragAndDropHUD::EDAD_Vender);
				m_slots.Add(obj);
			}
		}
	}
}

void AVender::SetCurrentPoint(const FString& Name)
{
	if (Name.IsEmpty()) return;

	TArray<AActor*> MovePoint;
	UGameplayStatics::GetAllActorsWithTag(this, FName("NpcMovePoint"), MovePoint);
	if (MovePoint.IsEmpty()) return;

	AOptionalTalkboxPoint* const find = Cast<AOptionalTalkboxPoint>(*MovePoint.FindByPredicate([&](AActor* const Point)->bool { 
			APatrolPoint* talkpoint = Cast<APatrolPoint>(Point);
			return talkpoint->GetUniqueName() == Name;
		}));

	if (find)
	{
		m_target_patrol = m_point_current = find;
		MoveToTarget(m_target_patrol);

		m_movement_component->MaxWalkSpeed = 600.f;
	}
}

const FString AVender::GetCurrentPointName() const
{
	return m_point_current ? m_point_current->GetUniqueName() : FString();
}

TArray<FString> const AVender::GetCameras() const
{
	TArray<FString> names;
	for (ADirectCamera* const camera : m_communicate_cameras)
	{
		if (camera) { names.Add(camera->GetUniqueName()); }
	}

	return names;
}

void AVender::SetCameras(const TArray<FString>& Cameras)
{
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsWithTag(this, FName("DirectCamera"), CameraActors);
	if (CameraActors.IsEmpty()) return;

	for (const FString& name : Cameras)
	{
		ADirectCamera* const find = Cast<ADirectCamera>(*CameraActors.FindByPredicate([&](AActor* const Camera)->bool { return Camera ? Cast< ADirectCamera>(Camera)->GetUniqueName() == name : false; }));
		if (find) { m_communicate_cameras.Add(find); }
	}
}

void AVender::SetTradeItems(const TArray<FInventorySaveData>& Items)
{
	m_slots.Reset();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	UDataTableManager* datatable_mgr = instance->GetSubsystem<UDataTableManager>();
	CHECK_INVALID_PTR(datatable_mgr)
	UDataTable* item_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Item);
	CHECK_INVALID_PTR(item_table)

	for (const FInventorySaveData& data : Items)
	{
		FItemData* item_data = item_table->FindRow<FItemData>(data.RowName, data.RowName.ToString());
		CHECK_INVALID_PTR(item_data)
		FString path = "WidgetBlueprint \'" + item_data->BluePrintPath + "\'";

		UClass* item_class = nullptr;
		if (item_data->EquipmentType == EEquipmentType::EET_Weapon) { item_class = LoadClass<AWeapon_Actor>(nullptr, *path); }
		else { item_class = LoadClass<AEquipment>(nullptr, *path); }

		TObjectPtr<UItemObject> item_object = NewObject<UItemObject>();
		item_object->SetItemProperties(data.RowName, *item_data, item_class, data.IsRotated);
		item_object->SetCurrentHUD(EDragAndDropHUD::EDAD_Vender);

		m_slots.Add(item_object);
	}
}

void AVender::OnCompleteGraceDiscover()
{
	if (!m_targets_patrol.IsEmpty())
	{
		m_target_patrol = m_point_current = m_targets_patrol[0];
		m_targets_patrol.RemoveAt(0);
	}

	CHECK_INVALID_PTR(m_target_patrol)
	MoveToTarget(m_target_patrol);
	++m_communicate_idx;

	FCommunicateScriptData* dialouge = m_dialouge_table->FindRow<FCommunicateScriptData>(FName("AfterFight_1"), FName("AfterFight_1").ToString());
	CHECK_INVALID_PTR(dialouge)
	m_dialogue_current = *dialouge;
	m_first_meet = false;

	m_movement_component->MaxWalkSpeed = 600.f;
}

const TObjectPtr<ACameraActor> AVender::GetCameraActor() const
{
	return m_communicate_cameras.IsValidIndex(m_communicate_idx) ? Cast<ACameraActor>(m_communicate_cameras[m_communicate_idx]) : nullptr;
}

void AVender::OnInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPlayer_Interface* player_interface = Cast<IPlayer_Interface>(OtherActor);
	if (OtherActor == this || !player_interface) return;

	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_interact_mgr->SetInterface(this);
	m_interact_mgr->OpenInteractPopUp(player_interface->GetControllerComponent(), EInteractPopupType::EIPT_ToNPC);

	CHECK_INVALID_SMART_PTR(m_trade_mgr)
	m_trade_mgr->SetVenderData(this);
}

void AVender::OnInteractEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;

	if (m_interact_mgr.IsValid())
	{
		m_interact_mgr->CloseInteractPopUp();
	}
}

void AVender::SaveInteractObjectData()
{
	if (false == m_attribute->IsAlive()) return;

	UERGameInstance* instance = GetGameInstance<UERGameInstance>();
	CHECK_INVALID_PTR(instance)

	instance->SaveInteractObject(this);
}

void AVender::MoveToSavePoint()
{
	CHECK_INVALID_PTR(m_target_patrol)
	MoveToTarget(m_target_patrol);

	m_movement_component->MaxWalkSpeed = 600.f;
}
