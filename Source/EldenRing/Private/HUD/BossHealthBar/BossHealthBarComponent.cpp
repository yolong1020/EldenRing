// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BossHealthBar/BossHealthBarComponent.h"
#include "HUD/BossHealthBar/BossHealthBar.h"
#include "Components/ProgressBar.h"
#include "Characters/Component/AttributeComponent.h"
#include "Characters/GameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Macro/DebugMacros.h"

UBossHealthBarComponent::UBossHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBossHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_tl_consume.TickTimeline(DeltaTime);
}

void UBossHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UClass* widget_class = LoadClass<UBossHealthBar>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/HUD/BossHealthBar/WBP_BossHealthBar.WBP_BossHealthBar_C'"));
	CHECK_INVALID_PTR(widget_class)

	m_health_bar = CreateWidget<UBossHealthBar>(GetWorld(), widget_class);
	CHECK_INVALID_PTR(m_health_bar)
}

void UBossHealthBarComponent::InitStatusBar(const FText& BossName, UAttributeComponent* const Attribute)
{
	m_attribute = Attribute;

	CHECK_INVALID_PTR(m_curve_consume)

	FOnTimelineFloat tl_callback;
	FOnUpdateStat consume_callback;
	FOnUpdateRecoverStat recovery_callback;

	tl_callback.BindUFunction(this, FName("UpdateConsumedHealth"));
	consume_callback.BindUFunction(this, FName("UpdateHealth"));
	recovery_callback.BindUFunction(this, FName("UpdateRecoverHealth"));

	m_health_bar->SetBossName(BossName);
	m_attribute->InitUpdateCallback(EOverlayStatType::EOST_Health, consume_callback, recovery_callback);
	m_tl_consume.AddInterpFloat(m_curve_consume, tl_callback);
}

void UBossHealthBarComponent::UpdateHealth(const float& Value)
{
	float percent = Value / m_attribute->GetHealthMax();
	m_health_bar->UpdateHealth(percent);

	if (m_is_delay) return;

	FTimerManager& timer_manager = GetWorld()->GetTimerManager();
	timer_manager.ClearTimer(m_delay);
	timer_manager.SetTimer(m_delay, this, &UBossHealthBarComponent::StartHealthTL, 1.4f);
	
	m_is_delay = true;
}

void UBossHealthBarComponent::UpdateRecoverHealth(const float& Value)
{
}

void UBossHealthBarComponent::UpdateConsumedHealth()
{
	if (m_health_bar->UpdateConsumedHealth())
	{
		m_tl_consume.Stop();
		m_is_delay = false;
	}
}

void UBossHealthBarComponent::StartHealthTL()
{
	m_tl_consume.PlayFromStart();
}

void UBossHealthBarComponent::ShowBossHealthBar()
{
	CHECK_INVALID_PTR(m_health_bar)
	m_health_bar->AddToViewport();
}

void UBossHealthBarComponent::HideBossHealthBar()
{
	if (m_health_bar) m_health_bar->RemoveFromParent();
}
