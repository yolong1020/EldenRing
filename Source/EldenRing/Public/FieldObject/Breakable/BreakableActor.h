// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/FieldObject.h"
#include "Common/Hit/HitInterface.h"
#include "FieldObject/FieldObject_Interface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;

UCLASS()
class ELDENRING_API ABreakableActor : public AFieldObject, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight) override final;

protected:
	virtual void BeginPlay() override final;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> m_capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> m_geometry_collection;
	
private:	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasure>> m_treasure_classes;

	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> m_sound_hit;

	bool m_is_broken = false;
};
