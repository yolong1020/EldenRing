// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOn.generated.h"

UCLASS()
class ELDENRING_API ULockOn : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPosition(const TWeakObjectPtr<APlayerController> Controller, const TWeakObjectPtr<class AGameCharacter> Target);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> MarkImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> SizeBox;

	UPROPERTY()
	TWeakObjectPtr<class AGameCharacter> m_target;

	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;
};
