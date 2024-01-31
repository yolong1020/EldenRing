// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyFelled.generated.h"

UCLASS()
class ELDENRING_API UEnemyFelled : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartDirecting();

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void ShowDirecting();

	UFUNCTION()
	void EndDirecting();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> ShowHideNotify;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_delay_time;
};

