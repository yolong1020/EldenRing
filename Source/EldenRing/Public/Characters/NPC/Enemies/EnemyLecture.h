// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPC_Character.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "EnemyLecture.generated.h"

UCLASS()
class BASIC_API AEnemyLecture : public ANPC_Character
{
	GENERATED_BODY()

public:
	AEnemyLecture();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight) override;

protected:
	virtual void BeginPlay() override;
};
