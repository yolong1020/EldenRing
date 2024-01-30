// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

UCLASS()
class ELDENRING_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPercent(const float& Percent);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> HealthBar;
};
