// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ERPlayerStart.generated.h"

UCLASS()
class ELDENRING_API AERPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	const FString& GetLocationName() const { return m_location_name; }
	
private:
	UPROPERTY(EditInstanceOnly, Category = "Location Name", meta = (AllowPrivateAccess = "true"))
	FString m_location_name;
};
