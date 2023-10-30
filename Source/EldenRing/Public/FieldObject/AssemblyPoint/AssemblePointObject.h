// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssemblePointObjectTypes.h"
#include "AssemblePointObject.generated.h"

class AAssemblyPoint;
class USphereComponent;
class UBoxComponent;
class ANPC_Character;

UCLASS()
class BASIC_API AAssemblePointObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AAssemblePointObject();

	virtual void Tick(float DeltaTime) override;
	virtual void InitData(AAssemblyPoint* const assembly_point);
	virtual void ResetPointObject();
	virtual void UsePointObject(ANPC_Character* const npc);

	FVector			GetActionLocation();
	const FString	GetInteractionSectionName();

	FORCEINLINE const bool&						IsUsing()				{ return m_is_using; }
	FORCEINLINE const EAssemblyPointObjectType& GetPointObjectType()	{ return m_point_object_type; }
	FORCEINLINE const double&					GetTimeStartUsing()		{ return m_time_using_start;}
	FORCEINLINE ANPC_Character* const			GetPointObjectUser()	{ return m_actor_rest; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_sphere_action;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Block")
	UBoxComponent* m_box_block;

	UPROPERTY(EditDefaultsOnly)
	EAssemblyPointObjectType m_point_object_type;

	UPROPERTY(EditInstanceOnly, Category = "Assembly Point Object User")
	ANPC_Character* m_actor_rest;

	AAssemblyPoint* m_assembly_point;
	bool			m_is_using;

	double m_time_using_start;
};
