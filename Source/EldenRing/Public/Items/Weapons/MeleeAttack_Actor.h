// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "MeleeAttack_Actor.generated.h"

UCLASS()
class BASIC_API AMeleeAttack_Actor : public AWeapon_Actor
{
	GENERATED_BODY()

public:
	AMeleeAttack_Actor();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return m_weapon_box; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
#pragma region Component
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_effect_sparks = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* m_weapon_box = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_boxtrace_start = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_boxtrace_end = nullptr;
#pragma endregion
};
