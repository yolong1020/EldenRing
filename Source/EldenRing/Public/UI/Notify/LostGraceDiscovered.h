// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LostGraceDiscovered.generated.h"

UCLASS()
class ELDENRING_API ULostGraceDiscovered : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartDirecting(const TObjectPtr<APlayerController>& Controller);

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void EndDirecting();

	UFUNCTION()
	void ShowDirecting();
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ShowHideNotify;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> m_controller;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_delay_time;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;
};
