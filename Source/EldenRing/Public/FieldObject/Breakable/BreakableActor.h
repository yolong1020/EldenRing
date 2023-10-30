// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;

UCLASS()
class BASIC_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* m_capsule = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* m_geometry_collection = nullptr;
	
private:	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasure>> m_treasure_classes;

	bool m_is_broken = false;
};
