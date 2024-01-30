// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "ObjectSpawnPoint.generated.h"

UCLASS()
class ELDENRING_API AObjectSpawnPoint : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	const FString& GetLocationName() const { return m_location_name; }

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Location Name", meta = (AllowPrivateAccess = "true"))
	FString m_location_name;
	
};
