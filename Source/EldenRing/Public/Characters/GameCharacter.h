// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CharacterTypes.h"
#include "GameCharacter.generated.h"

class UAnimMontage;
class UAttributeComponent;
class USphereComponent;
class UPawnSensingComponent;
class ULockOnComponent;
class AWeapon_Actor;

UCLASS()
class BASIC_API AGameCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AGameCharacter();

	virtual void	Tick(float DeltaTime) override;
	virtual void	Destroyed() override;

	virtual void	SetWeaponCollision(ECollisionEnabled::Type type) {};
	virtual void	SetParryCollision(const ECollisionEnabled::Type& type);

	virtual void	GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight) override {};
	virtual float	TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void	TakeExecution(AActor* target, const EGameDirection& direction, const int32& damage) {};

	virtual bool	IsCurrentActionState(const FString& action) { return false; }
	virtual bool	IsGuardState() { return false; }

	virtual void	SetExecutionEnable(const bool& is_enable, AGameCharacter* const execution_target = nullptr);
	virtual void	SetActiveLockOn(const bool& is_visible);
	
	virtual void	OnAttackDefended(const EAttackWeight& attack_weight) {};
	virtual void	OnAttackBlocked(const EAttackWeight& attack_weight) {};
	virtual void	OnTakeStunn();
	virtual void	OnEndStunn();
	virtual void	OnParryStart() {};
	virtual void	OnParryEnd() {};
	virtual void	OnTakeExecutionEnd();
	virtual void	OnReactEnd() {};

	virtual FVector	GetBoneLocation(const FName& bone_name);
	FORCEINLINE EEquipState			GetEquipState()		{ return m_equip_state; }
	FORCEINLINE USphereComponent* const	GetParrySphere()	{ return m_sphere_parry; }
	FORCEINLINE const EDeathPose&		GetDeathPose()		{ return m_death_pose; }
	FORCEINLINE const EBattlePose&		GetBattlePose()		{ return m_battle_pose; }

protected:
	virtual void BeginPlay() override;

	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) {};
	virtual void RefreshHealthbar(float DamageAmount = 0) {};

	void PlayMontage(UAnimMontage* montage, const FName& section_name, const float& play_rate = 1.0, const bool& stop_all_montage = false, const float& blend_out = 0.25);
	void StopAllMontage(const float& weight = 0.25f);
	void ChangeRootMotionMode(ERootMotionMode::Type mode);

	UFUNCTION()
	virtual void OnParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void InExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OutExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EGameDirection FindDirection(const AActor* standard, const FVector& target);

protected:
	USkeletalMeshComponent* m_mesh = nullptr;

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_death = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_attack = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_potion_consume = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_turn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_blocked = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_guard = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_stunn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_execution = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_take_execution = nullptr;
#pragma endregion

#pragma region Combat
	UPROPERTY(VisibleInstanceOnly)
	AGameCharacter*	m_actor_target = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	AGameCharacter*	m_actor_execution_target = nullptr;

	UPROPERTY(VisibleAnywhere)
	ULockOnComponent* m_widget_lockon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Parry)
	USphereComponent* m_sphere_parry = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = execution)
	TArray<USphereComponent*> m_sphere_execution;

	FName		m_attack_name_prev 	= "";
	bool		m_attack_success	= false;
	bool		m_is_enable_execution	= false;
	int32		m_lockon_index 		= 0;

	EAttackType	m_attack_type_prev	= EAttackType::EATKT_None;
	EAttackStrength m_attack_strength	= EAttackStrength::EATKS_None;
	EBattlePose	m_battle_pose		= EBattlePose::EBP_Unoccupied;
#pragma endregion

#pragma region Interaction
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	USphereComponent* m_sphere_interaction = nullptr;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* m_pawn_sensing = nullptr;
#pragma endregion

#pragma region States
	UPROPERTY(BlueprintReadOnly, Category = "States")
	EDeathPose m_death_pose = EDeathPose::EDP_Alive;

	UPROPERTY(EditInstanceOnly, Category = "States")
	EEquipState m_equip_state = EEquipState::EES_Unequipped;
#pragma endregion

#pragma region Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon_Actor> m_class_weapon_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon_Actor> m_class_weapon_L;

	AWeapon_Actor* m_equiped_weapon_R = nullptr;
	AWeapon_Actor* m_equiped_weapon_L = nullptr;
#pragma endregion

#pragma region Attribute
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* m_attribute = nullptr;
#pragma endregion

#pragma region Sound
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* m_sound_hit = nullptr;
#pragma endregion

#pragma region Particle
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* m_particle_hit = nullptr;
#pragma endregion
};
