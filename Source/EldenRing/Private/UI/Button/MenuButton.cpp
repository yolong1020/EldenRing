// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/MenuButton.h"
#include "UI/UIDelegates.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/AudioComponent.h"
#include "HUD/GameHUD_Actor.h"
#include "GameBase/ERGameInstance.h"
#include "System/UIManager.h"
#include "System/SoundscapeManager.h"
#include "TimerManager.h"
#include "Macro/DebugMacros.h"

void UMenuButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CHECK_INVALID_PTR(Button)
	Button->OnClicked.AddDynamic(this, &UMenuButton::ButtonFunc);
}

void UMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonText(TextContext);
}

void UMenuButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (!m_check_activate.IsBound() || m_check_activate.Execute() || !m_check_directing.IsBound() || m_check_directing.Execute()) return;

	CHECK_INVALID_PTR(Hover)
	Hover->SetRenderOpacity(1.f);

	if (HoverSound) 
	{ 
		UAudioComponent* component = UGameplayStatics::CreateSound2D(this, HoverSound); 
		component->VolumeMultiplier = 0.05;
		component->bAllowSpatialization = false;
		component->Play();
	}
}

void UMenuButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (!m_check_activate.IsBound() || m_check_activate.Execute()) return;

	CHECK_INVALID_PTR(Hover)
	Hover->SetRenderOpacity(0.f);
}

void UMenuButton::ButtonFunc()
{
	if (!m_check_activate.IsBound() || m_check_activate.Execute() || !m_check_directing.IsBound() || m_check_directing.Execute()) return;

	if (ButtonType == EMenuButtonType::EMBT_LoadGame)
	{
		if (false == UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
		{
			UGameInstance* instance = GetGameInstance();
			CHECK_INVALID_PTR(instance)

			UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
			CHECK_INVALID_PTR(ui_mgr)
			ui_mgr->OpenSubNotify(FText::FromString(TEXT("세이브 파일을 찾을 수 없습니다.")));
			return;
		}
	}

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller)
	{
		controller->DisableInput(controller);
		controller->bShowMouseCursor = false;
	}

	float duration = 1.f;
	if (ClickSound)
	{ 
		UAudioComponent* component = UGameplayStatics::CreateSound2D(this, ClickSound); 
		CHECK_INVALID_PTR(component)
		component->bAllowSpatialization = false;
		component->Play();
		duration = ClickSound->GetDuration();
	}

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	FTimerManager& time_manager = world->GetTimerManager();
	time_manager.ClearTimer(m_timer);
	time_manager.SetTimer(m_timer, this, &UMenuButton::OpenNextLevel, duration * 0.5f);

	m_activated.Execute(true);
}

void UMenuButton::OpenNextLevel()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	FTimerManager& time_manager = world->GetTimerManager();
	time_manager.ClearTimer(m_timer);

	FAfterShowBlackOutFunc callback;
	if (ButtonType == EMenuButtonType::EMBT_Exit) { callback.BindLambda([&]()->void { UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false); }); }
	else 
	{
		callback.BindLambda([&]()->void {
			UERGameInstance* instance = Cast<UERGameInstance>(UGameplayStatics::GetGameInstance(this));
			CHECK_INVALID_PTR(instance)
			instance->OpenLevel("/Game/Maps/GameField", FName("GameField"));
		});
	}

	UERGameInstance* instance = Cast<UERGameInstance>(UGameplayStatics::GetGameInstance(this));
	CHECK_INVALID_PTR(instance)
	instance->SetAsNewGame(ButtonType == EMenuButtonType::EMBT_NewGame);

	USoundscapeManager* sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->StopStageBGM();

	UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_PTR(ui_mgr)
	ui_mgr->ShowBlackOut(callback);
}
