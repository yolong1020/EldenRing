// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/InputActionType.h"
#include "TutorialCallout.generated.h"

class UTutorialHUD;

UCLASS()
class ELDENRING_API UTutorialCallout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const bool IsActivated() const;
	FORCEINLINE const bool IsActivated(const FName& Name) const;

	void OriginalSetUp(const FName& Name, const EInputActionType& Type, const bool IsLargeKey, const float& Duration = 0);
	void HideInteraction();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTutorialHUD> UIP_SlideTop;
};
