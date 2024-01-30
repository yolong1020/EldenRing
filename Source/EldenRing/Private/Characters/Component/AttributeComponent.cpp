// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Component/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/DataTableManager.h"
#include "Items/ItemObject.h"
#include "Macro/DebugMacros.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_tl_recovery.SetNum((int32)EOverlayStatType::EOST_End);
	m_consume.SetNum((int32)EOverlayStatType::EOST_End);
	m_recover.SetNum((int32)EOverlayStatType::EOST_End);
	m_delay.SetNum((int32)EOverlayStatType::EOST_End);

	//	Timeline
	FOnTimelineFloat tl_health_callback;
	FOnTimelineFloat tl_mana_callback;
	FOnTimelineFloat tl_stamina_callback;

	tl_health_callback.BindUFunction(this, FName("UpdateRecoverHealth"));
	tl_mana_callback.BindUFunction(this, FName("OnUpdateRegenerateStat_Mana"));
	tl_stamina_callback.BindUFunction(this, FName("UpdateRecoverStamina"));

	if (m_curve_regenerate)
	{
		m_tl_recovery[(int32)EOverlayStatType::EOST_Health].AddInterpFloat(m_curve_regenerate, tl_health_callback);
		m_tl_recovery[(int32)EOverlayStatType::EOST_Mana].AddInterpFloat(m_curve_regenerate, tl_mana_callback);
		m_tl_recovery[(int32)EOverlayStatType::EOST_Stamina].AddInterpFloat(m_curve_regenerate, tl_stamina_callback);
	}

	m_delay[(int32)EOverlayStatType::EOST_Health].CallbackTarget	 = this;
	m_delay[(int32)EOverlayStatType::EOST_Health].Linkage			 = (int32)EOverlayStatType::EOST_Health;
	m_delay[(int32)EOverlayStatType::EOST_Health].ExecutionFunction  = FName("StartHealthTL");
	m_delay[(int32)EOverlayStatType::EOST_Health].UUID				 = FGuid::NewGuid().A;
																	 
	m_delay[(int32)EOverlayStatType::EOST_Stamina].CallbackTarget	 = this;
	m_delay[(int32)EOverlayStatType::EOST_Stamina].Linkage			 = (int32)EOverlayStatType::EOST_Stamina;
	m_delay[(int32)EOverlayStatType::EOST_Stamina].ExecutionFunction = FName("StartStaminaTL");
	m_delay[(int32)EOverlayStatType::EOST_Stamina].UUID				 = FGuid::NewGuid().A;

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)

	m_data_mgr = instance->GetSubsystem<UDataTableManager>();
	CHECK_INVALID_SMART_PTR(m_data_mgr)
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FTimeline timeline : m_tl_recovery) { timeline.TickTimeline(DeltaTime); }
}

void UAttributeComponent::InitUpdateCallback(const EOverlayStatType& Type, const FOnUpdateStat& Consume, const FOnUpdateRecoverStat& Recover)
{
	if (Type == EOverlayStatType::EOST_End) return;

	m_consume[(int32)Type] = Consume; m_recover[(int32)Type] = Recover;
}

void UAttributeComponent::AddOffset(const TObjectPtr<class UItemObject> Item)
{
	if (m_offsets.Find(Item)) return;

	CHECK_INVALID_SMART_PTR(m_data_mgr)
	UDataTable* data_table = m_data_mgr->GetDataTable(EDataTableType::EDTT_ItemProp);
	CHECK_INVALID_PTR(data_table)

	FName name = Item->GetRowName();
	FItemProperties* properties = data_table->FindRow<FItemProperties>(name, name.ToString());
	CHECK_INVALID_PTR(properties)

	m_offsets.Add(Item, *properties);
	m_health_max  += properties->Hp;
	m_stamina_max += properties->Stamina;
}

void UAttributeComponent::RemoveOffset(const TObjectPtr<class UItemObject> Item)
{
	if (!m_offsets.Find(Item)) return;

	float health  = m_health_max - m_offsets[Item].Hp;
	float stamina = m_stamina_max - m_offsets[Item].Stamina;

	if (health < m_health_current)   m_health_current  = health;
	if (stamina < m_stamina_current) m_stamina_current = stamina;

	m_health_max  = health;
	m_stamina_max = stamina;

	m_offsets.Remove(Item);
}

void UAttributeComponent::ResetHealthToFull()
{
	m_health_current = m_health_max;
	m_recover[(int32)EOverlayStatType::EOST_Health].Execute(m_health_current);
}

void UAttributeComponent::ResetManaToFull()
{
	m_mana_current = m_mana_max;
	m_recover[(int32)EOverlayStatType::EOST_Mana].Execute(m_mana_current);
}

void UAttributeComponent::ResetStaminaToFull()
{
	m_stamina_current = m_stamina_max;
	m_recover[(int32)EOverlayStatType::EOST_Stamina].Execute(m_stamina_current);
}

void UAttributeComponent::StartHealthTL()
{
	m_tl_recovery[(int32)EOverlayStatType::EOST_Health].PlayFromStart();
}

void UAttributeComponent::StartStaminaTL()
{
	m_tl_recovery[(int32)EOverlayStatType::EOST_Stamina].PlayFromStart();
}

void UAttributeComponent::UpdateRecoverHealth(float curve_value)
{
	m_health_current = FMath::Clamp(m_health_current + 2.f, 0, m_health_goal);
	m_recover[(int32)EOverlayStatType::EOST_Health].Execute(m_health_current);

	if (m_health_current >= m_health_goal)
	{
		m_health_current = m_health_goal;

		m_tl_recovery[(int32)EOverlayStatType::EOST_Health].Stop();
	}
}

void UAttributeComponent::UpdateRecoverMana(float curve_value)
{
}

void UAttributeComponent::UpdateRecoverStamina(float curve_value)
{
	m_stamina_current = FMath::Clamp(m_stamina_current + 1.f, 0, m_stamina_max);
	m_recover[(int32)EOverlayStatType::EOST_Stamina].Execute(m_stamina_current);

	if (m_stamina_current == m_stamina_max)
	{
		m_tl_recovery[(int32)EOverlayStatType::EOST_Stamina].Stop();
	}
}

void UAttributeComponent::RecoverHealth(float Amount)
{
	m_health_goal = m_health_current + Amount;
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0, m_delay[(int32)EOverlayStatType::EOST_Health]);
}

void UAttributeComponent::RecoverStamina()
{
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0, m_delay[(int32)EOverlayStatType::EOST_Stamina]);
}

void UAttributeComponent::ReceiveDamage(float damage)
{
	if (0 >= damage) return;

	m_tl_recovery[(int32)EOverlayStatType::EOST_Health].Stop();

	m_health_current = FMath::Clamp(m_health_current - damage, 0, m_health_max);
	m_consume[(int32)EOverlayStatType::EOST_Health].Execute(m_health_current);
}

void UAttributeComponent::ConsumeStamina(float amount)
{
	if (0 >= amount) return;

	m_tl_recovery[(int32)EOverlayStatType::EOST_Stamina].Stop();

	m_stamina_current = FMath::Clamp(m_stamina_current - amount, 0, m_stamina_max);
	m_consume[(int32)EOverlayStatType::EOST_Stamina].Execute(m_stamina_current);
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 2, m_delay[(int32)EOverlayStatType::EOST_Stamina]);
}

