// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Loading.generated.h"

UCLASS()
class ELDENRING_API ULoading : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenLoading();
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;

private:
	UPROPERTY(BlueprintReadWrite, Category = "Progress Bar", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> Progress;

	UPROPERTY(BlueprintReadWrite, Category = "Slider Point", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USlider> Thumb;

	bool m_is_triggered;
};
