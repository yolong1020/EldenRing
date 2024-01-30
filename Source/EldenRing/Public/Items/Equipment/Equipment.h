// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Equipment.generated.h"

UCLASS()
class ELDENRING_API AEquipment : public AItem
{
	GENERATED_BODY()
	
public:
	AEquipment();
	virtual void Tick(float DeltaTime) override;

	void Equip(USceneComponent* parent, AActor* owner, APawn* instigator);
	void AttachMashToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* m_mesh = nullptr;
};
