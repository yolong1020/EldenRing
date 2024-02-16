// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/GameField.h"
#include "GameBase/ERGameInstance.h"
#include "GameBase/ERGameMode.h"
#include "System/SoundscapeManager.h"
#include "System/UIManager.h"
#include "Blueprint/UserWidget.h"
#include "HUD/GameHUD_Actor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "UI/Loading/Loading.h"
#include "Macro/DebugMacros.h"

void AGameField::LoadedLevel()
{
	GetWorldTimerManager().ClearTimer(timer_start);
	GetWorldTimerManager().SetTimer(timer_start, this, &AGameField::StartStage, 6.f);
}

void AGameField::BeginPlay()
{
	Super::BeginPlay();
	m_is_play = true;

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_PTR(ui_mgr);

	FAfterShowBlackOutFunc show_callback;
	show_callback.BindLambda([&]()->void {
			UClass* widget_class = LoadClass<ULoading>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Loading/WBP_Loading.WBP_Loading_C'"));
			CHECK_INVALID_PTR(widget_class)

			m_widget = CreateWidget<ULoading>(GetWorld(), widget_class);
			m_widget->OpenLoading();

			UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
			CHECK_INVALID_PTR(instance)
			UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
			CHECK_INVALID_PTR(ui_mgr);

			FAfterHideBlackOutFunc hide_callback;
			hide_callback.BindLambda([&]()->void { LoadDataLayer(); });
			ui_mgr->HideBlackOut(hide_callback);
		});
	ui_mgr->ShowBlackOut(show_callback);

	UMaterialParameterCollection* collection = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("MaterialParameterCollection'/Game/AncientContent/Materials/MPC_3DMaster.MPC_3DMaster'"));
	CHECK_INVALID_PTR(collection)
	
	UKismetMaterialLibrary::SetScalarParameterValue(this, collection, FName("Dark World Transition"), 1.f);
}

void AGameField::StartStage()
{
	UWorld* world = GetWorld();
	AERGameMode* game_mode = Cast<AERGameMode>(world->GetAuthGameMode());
	if (game_mode)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0);
		AActor* start = game_mode->FindPlayerStart(controller);
		if (start)
		{
			game_mode->SpawnDefaultPawnAtTransform(controller, start->GetActorTransform());
		}
	}

	USoundBase* sound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/EldenRing/Sounds/BackgroundMusic/Field.Field'"));
	CHECK_INVALID_PTR(sound)
	m_sound_mgr->PlayStageBGM(sound);

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	if (false == instance->IsNewGame()) { instance->LoadLevelObjects(); }

	UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_PTR(ui_mgr)
	FAfterHideBlackOutFunc callback;
	callback.BindLambda([&, instance, ui_mgr]()->void {

			instance->RequestLevelLoadCompleteEvent();

			if (instance->IsNewGame())
			{
				TArray<FTutorialPersistentData> datas;
				FTutorialPersistentData Dodge(FName("Dodge"), EInputActionType::EIAT_Dodge, ETutorialPosition::ETP_Top, true);
				datas.Add(Dodge);
				FTutorialPersistentData Sprint(FName("Sprint(Hold)"), EInputActionType::EIAT_Sprint, ETutorialPosition::ETP_Mid, true);
				datas.Add(Sprint);
				FTutorialPersistentData Walk(FName("Walk(Hold)"), EInputActionType::EIAT_Walk, ETutorialPosition::ETP_Bot, true);
				datas.Add(Walk);
	
				ui_mgr->OpenTutorialGroupCall(datas);
			}
		});
	ui_mgr->HideBlackOut(callback);
}
