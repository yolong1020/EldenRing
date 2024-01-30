// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

class USoundBase;

UCLASS()
class ELDENRING_API ATreasure : public AItem
{
	GENERATED_BODY()
	
public:
	ATreasure();

	void InitDroppedTreasureFromPlayer(const int32& amount);
	FORCEINLINE void InitGoldAmount(const int32& amount) { m_value = amount; }


protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_mesh;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase*	m_sound_pickup = nullptr;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32	m_value_min;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32	m_value_max;
	
	int32	m_value;
	bool	m_is_dropped_from_player;
};
