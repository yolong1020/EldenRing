// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "FieldObject/FieldObject.h"
#include "PatrolPoint.generated.h"

UCLASS()
class ELDENRING_API APatrolPoint : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FString& GetUniqueName() const	{ return m_unique_name; }
	FORCEINLINE void SetUniqueName(const FString& Name) { m_unique_name = Name; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Unique Name")
	FString m_unique_name;
};
