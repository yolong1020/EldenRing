// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment/EquipmentTypes.h"
#include "Item.generated.h"

class  UItemObject;
class  USphereComponent;
class  UNiagaraComponent;

struct FItemData;

UCLASS()
class BASIC_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;
	virtual void SetSphereCollisionActivation(ECollisionEnabled::Type type);
	FORCEINLINE UItemObject* GetItemObject() const { return m_item_object; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_effect_embers;
#pragma endregion

#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	UItemObject* 	m_item_object;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FName		m_item_name;
	FItemData*	m_item_data;
#pragma endregion
};
