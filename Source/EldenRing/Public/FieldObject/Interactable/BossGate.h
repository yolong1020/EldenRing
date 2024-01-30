// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/Interactable/InteractableObject.h"
#include "BossGate.generated.h"

UCLASS()
class ELDENRING_API ABossGate : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	explicit ABossGate();

	virtual void DoFirstInteract() override final;
	FORCEINLINE virtual const FString GetInteractorName() const	override final { return m_interactor_name; }

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaSeconds) override final;

	virtual void InInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override final;
	virtual void OutInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override final;

private:
	UFUNCTION()
	void ToggleImpactFX();

	UFUNCTION()
	void ToggleBlock();

	UFUNCTION()
	void ResetBlock();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_blind_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detect Area", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* m_sphere_detect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blocker", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* m_box_block;

	UPROPERTY(EditInstanceOnly, Category = "Location Name", meta = (AllowPrivateAccess = "true"))
	FString m_interactor_name;

	bool m_is_detacted_enemy;
};
