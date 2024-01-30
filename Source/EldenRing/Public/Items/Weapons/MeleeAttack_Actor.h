// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "MeleeAttack_Actor.generated.h"

UCLASS()
class ELDENRING_API AMeleeAttack_Actor : public AWeapon_Actor
{
	GENERATED_BODY()

public:
	AMeleeAttack_Actor();
	virtual void SetWeaponCollision(const ECollisionEnabled::Type& Type) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* m_weapon_box_extra;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* m_boxtrace_start_extra;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* m_boxtrace_end_extra;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* m_weapon_box;

private:
#pragma region Component
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_effect_sparks;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* m_boxtrace_start;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* m_boxtrace_end;
#pragma endregion
};
