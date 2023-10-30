// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOn.generated.h"

class UImage;
class UCanvasPanel;

UCLASS()
class BASIC_API ULockOn : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	UImage* MarkImage;
};
