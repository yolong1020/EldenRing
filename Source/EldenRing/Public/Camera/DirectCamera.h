// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "DirectCamera.generated.h"

UCLASS()
class ELDENRING_API ADirectCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FString& GetUniqueName() const	{ return m_unique_name; }
	FORCEINLINE void SetUniqueName(const FString& Name) { m_unique_name = Name; }

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Unique Name")
	FString m_unique_name;
};
