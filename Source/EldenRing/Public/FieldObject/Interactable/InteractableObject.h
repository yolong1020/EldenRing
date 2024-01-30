// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/PopUp/PopUpTypes.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "InteractableObject.generated.h"

UCLASS(Abstract)
class ELDENRING_API AInteractableObject : public AActor, public IInteractor_Interface
{
	GENERATED_BODY()
	
public:	
	explicit AInteractableObject();
	virtual void Destroyed() override final;

	FORCEINLINE virtual const bool IsFirstInteract() const override final			{ return m_is_first; }
	FORCEINLINE virtual void SetFirstInteract(const bool IsFirst) override final	{ m_is_first = IsFirst; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void InInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) PURE_VIRTUAL(AInteractableObject::InInteractOverlap, );

	UFUNCTION()
	virtual void OutInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) PURE_VIRTUAL(AInteractableObject::InInteractOverlap,);

	UFUNCTION()
	virtual void SaveInteractObjectData();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Root")
	UStaticMeshComponent* m_mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	class USphereComponent* m_sphere_interaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	class UBoxComponent* m_box_interaction;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UObserverManager> m_observer_mgr;

	bool m_is_first;
};
