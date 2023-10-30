// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/EnemyLecture.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Basic/DebugMacros.h"

AEnemyLecture::AEnemyLecture()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyLecture::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyLecture::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyLecture::GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight)
{
	//DirectionalHitReact(ImpactPoint);

	if (nullptr == m_sound_hit) return;

	UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, ImpactPoint);
	
	auto world = GetWorld();
	if (nullptr == world) return;

	if (nullptr == m_particle_hit) return;
	UGameplayStatics::SpawnEmitterAtLocation(
		world, 
		m_particle_hit, 
		ImpactPoint);

#ifdef _DEBUG
	DRAW_SPHERE_DURATION(ImpactPoint)
#endif
}

