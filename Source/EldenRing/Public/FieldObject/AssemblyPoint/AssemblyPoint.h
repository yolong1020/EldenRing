// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssemblyPoint.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class UParticleSystemComponent;
class AAssemblePointObject;
class ANPC_Character;

UCLASS()
class BASIC_API AAssemblyPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AAssemblyPoint();

	virtual void Tick(float DeltaTime) override;

	bool RegisterAssemblyPointMember(ANPC_Character* const new_member);
	AAssemblePointObject* const TryUsePointObject(ANPC_Character* const request_npc);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Affect Navigationi", meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_sphere_block;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_fire_particle;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_smoke_particle;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	USoundBase* m_fire_sound;

	UPROPERTY(EditDefaultsOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	int32 m_members_limit_count;

	UPROPERTY(VisibleInstanceOnly, Category = "Assembly Point Properties", meta = (AllowPrivateAccess = "true"))
	TArray<ANPC_Character*> m_registed_members;

	UPROPERTY(EditInstanceOnly, Category = "Assembly Point", meta = (AllowPrivateAccess = "true"))
	TArray<AAssemblePointObject*> m_assembly_objects;
};
