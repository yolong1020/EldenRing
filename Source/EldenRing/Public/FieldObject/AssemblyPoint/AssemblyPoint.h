// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/FieldObject.h"
#include "System/Event/EventDelegates.h"
#include "AssemblyPoint.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class UParticleSystemComponent;
class AAssemblePointObject;
class ANPC_Character;

UCLASS()
class ELDENRING_API AAssemblyPoint : public AFieldObject
{
	GENERATED_BODY()
	
public:	
	AAssemblyPoint();

	UFUNCTION()
	void ReservateRest(ANPC_Character* const NewMember);

	UFUNCTION()
	void PermissionUsing(AAssemblePointObject* const PointObject);

	bool RegisteAssemblyPointMember(ANPC_Character* const NewMember, FRequestAllowUsing& Callback);
	void UnregistAssemblyPointMember(ANPC_Character* const Member);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Affect Navigationi", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* m_capsule_block;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_fire_particle;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_smoke_particle;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	int32 m_members_limit_count;

	UPROPERTY(VisibleInstanceOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	TArray<ANPC_Character*> m_registed_members;

	UPROPERTY(EditInstanceOnly, Category = "Assembly Point", meta = (AllowPrivateAccess = "true"))
	TArray<AAssemblePointObject*> m_assembly_objects;

	TArray<ANPC_Character*> m_request_list;
	FWaitAllowUsing m_wait_event;
};
