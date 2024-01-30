// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/FieldObject.h"
#include "AssemblePointObjectTypes.h"
#include "System/Event/EventDelegates.h"
#include "AssemblePointObject.generated.h"

class AAssemblyPoint;
class USphereComponent;
class UBoxComponent;
class ANPC_Character;

UCLASS()
class ELDENRING_API AAssemblePointObject : public AFieldObject
{
	GENERATED_BODY()
	
public:	
	AAssemblePointObject();

	void UsePointObject(ANPC_Character* const NPC);

	FVector			GetActionLocation();
	const FString	GetInteractionSectionName();

	void InitCallback(const FReportUsingPointObject& Callback);
	void ResetPointObject();
	const bool UseAgainPointObject();

	FORCEINLINE const EAssemblyPointObjectType& GetPointObjectType() { return m_point_object_type; }
	FORCEINLINE ANPC_Character* const			GetPointObjectUser() { return m_actor_rest; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UFUNCTION()
	void ReportUsingTime();

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_sphere_action;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Block", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* m_box_block;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	EAssemblyPointObjectType m_point_object_type;

	UPROPERTY(EditInstanceOnly, Category = "Assembly PointObject User", meta = (AllowPrivateAccess = "true"))
	ANPC_Character* m_actor_rest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Limit Time", meta = (AllowPrivateAccess = "true"))
	double m_time_limit;

	FLatentActionInfo	m_latent;
	FReportUsingPointObject m_report_callback;
	bool m_is_using;
	bool m_is_reserved;

	double m_time_using_start;
};
