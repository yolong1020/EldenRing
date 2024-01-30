// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/Interactable/InteractableObject.h"
#include "Bonefire.generated.h"

class UParticleSystemComponent;

UCLASS(Blueprintable)
class ELDENRING_API ABonefire : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	explicit ABonefire();

	virtual void DoFirstInteract() override final;
	FORCEINLINE virtual const TObjectPtr<ACameraActor> GetCameraActor() const override final	{ return m_camera; }
	FORCEINLINE virtual const FString GetInteractorName() const	override final					{ return m_interactor_name; }

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaSeconds) override final;

	virtual void InInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override final;
	virtual void OutInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override final;

private:
	UFUNCTION()
	void ToggleImpactFX();

private:
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class ACameraActor* m_camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_crystal_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_halo_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_reflection_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_impact_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* m_ember_particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detect Area", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* m_sphere_detect;

	UPROPERTY(EditInstanceOnly, Category = "Location Name", meta = (AllowPrivateAccess = "true"))
	FString m_interactor_name;
	
	bool m_is_detacted_enemy;
};
