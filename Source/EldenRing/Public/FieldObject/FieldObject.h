// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldObject_Interface.h"
#include "FieldObject.generated.h"

UCLASS()
class ELDENRING_API AFieldObject : public AActor, public IFieldObject_Interface
{
	GENERATED_BODY()
	
public:	
	explicit AFieldObject();
	virtual void Destroyed() override final;

	UFUNCTION()
	void SaveFieldObjectData();

	FORCEINLINE virtual void  SetFieldObjectTransform(const FTransform& Transform) override final	{ SetActorTransform(Transform); m_start_transform = Transform; }
	
	FORCEINLINE virtual const FTransform GetFieldObjectTransform() const override final				{ return m_start_transform; }
	FORCEINLINE virtual TSubclassOf<class AFieldObject> GetClassType() const						{ return GetClass(); }

	FORCEINLINE const FString& GetUniqueName() const												{ return m_unique_name; }
	FORCEINLINE void SetUniqueName(const FString& Name)												{ m_unique_name = Name; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TWeakObjectPtr<class UObserverManager> m_observer_mgr;

	UPROPERTY(EditInstanceOnly, Category = "Unique Name")
	FString m_unique_name;

	FTransform m_start_transform;
};
