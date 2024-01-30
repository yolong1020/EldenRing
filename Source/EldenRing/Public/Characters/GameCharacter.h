// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/Hit/HitInterface.h"
#include "Characters/GameCharacter_Interface.h"
#include "CharacterTypes.h"
#include "Components/TimelineComponent.h"
#include "GameCharacter.generated.h"

class UAnimMontage;
class UAttributeComponent;
class USphereComponent;
class UPawnSensingComponent;
class AWeapon_Actor;

DECLARE_MULTICAST_DELEGATE(FBodyAttackEvent)

UCLASS(Abstract, Blueprintable)
class ELDENRING_API AGameCharacter : public ACharacter, public IHitInterface, public IGameCharacter_Interface
{
	GENERATED_BODY()

public:
	AGameCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
			
	virtual void TakeExecution(AActor* target, const EGameDirection& direction, const int32& damage) PURE_VIRTUAL(AGameCharacter::TakeExecution, );

	virtual void OnAttackDefended(const EAttackWeight& attack_weight) PURE_VIRTUAL(AGameCharacter::OnAttackDefended, );
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) PURE_VIRTUAL(AGameCharacter::OnAttackBlocked, );
	virtual void OnTakeStunn();
	virtual void OnEndStunn();
	virtual void OnTakeExecutionEnd();
	virtual void OnReactEnd() PURE_VIRTUAL(AGameCharacter::OnReactEnd, );

	FORCEINLINE virtual void										AddIgnoreTargets(AActor* const Target) override	{ m_ignore_target.Add(Target); }
	
	virtual const bool												IsAttacking()								const PURE_VIRTUAL(AGameCharacter::IsAttacking, return false; );
	virtual const bool												IsCurrentActionState(const FString& action) const PURE_VIRTUAL(AGameCharacter::IsCurrentActionState, return false; );
	virtual const bool												IsGuardState()							    const PURE_VIRTUAL(AGameCharacter::IsGuardState, return false; );
	FORCEINLINE virtual const bool									IsNotDamageMod() const override					{ return m_is_not_damage_mod; }
	FORCEINLINE virtual const bool									IsHasTag(const FName& TagName) const override	{ return ActorHasTag(TagName); }
	FORCEINLINE virtual const bool									IsShowingWeapon() const override final;
	const bool														IsAlive() const;
	
	virtual const FVector											GetBoneLocation(const FName& BoneName, const EBoneSpaces::Type& Space);
	FORCEINLINE const FString&										GetUniqueName() const							{ return m_unique_name; }
	FORCEINLINE virtual const TArray<AActor*>						GetIgnoreTargets() const override final			{ return m_ignore_target; }
	FORCEINLINE virtual const int32									GetTotalGold() const override	final			{ return m_total_gold; }
	FORCEINLINE USphereComponent* const								GetParrySphere()								{ return m_sphere_parry; }
	FORCEINLINE const EDeathPose&									GetDeathPose()									{ return m_death_pose; }
	FORCEINLINE const EBattlePose&									GetBattlePose()									{ return m_battle_pose; }
	FORCEINLINE virtual const FTransform							GetStartTransform() const override final		{ return m_start_transform; }
	FORCEINLINE virtual const uint8									GetWeaponEquipState() const override final		{ return (uint8)m_equip_state; }
	FORCEINLINE virtual TSubclassOf<AGameCharacter>					GetClassType() const override final				{ return GetClass(); }
	FORCEINLINE virtual TArray<TSubclassOf<class AWeapon_Actor>>	GetWeaponClassTypes() const;
	
	FORCEINLINE void			SetGold(const int32& gold)													{ m_total_gold += gold; }
	FORCEINLINE virtual void	SetDamageMod(const bool& IsNoDamage)										{ m_is_not_damage_mod = IsNoDamage; }
	FORCEINLINE virtual void	SetStartTransform(const FTransform& Transform)								{ m_start_transform = Transform; }
	FORCEINLINE virtual void	SetCharacterTransform(const FTransform& Transform) override final			{ SetActorTransform(Transform); m_start_transform = Transform; }
	FORCEINLINE virtual void	SetWeaponEquipState(const EEquipState& WeaponEquipState) override final		{ m_equip_state = WeaponEquipState; }
	virtual void				SetWeaponCollision(ECollisionEnabled::Type type) PURE_VIRTUAL(AGameCharacter::SetWeaponCollision, );
	virtual void				SetParryCollision(const ECollisionEnabled::Type& type);
	virtual void				SetExecutionEnable(const bool& IsFront, const bool& is_enable, AGameCharacter* const execution_target = nullptr);

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration = 0);

	UFUNCTION()
	virtual void UninitAndDestroy();
	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) PURE_VIRTUAL(AGameCharacter::HitReact, );
	virtual void RefreshHealthbar(float DamageAmount = 0) PURE_VIRTUAL(AGameCharacter::RefreshHealthbar, );
	virtual void InitBodyAttackIgnoreCallback();

	void PlayMontageSection(UAnimMontage* const Montage, const FName& SectionName, const float& PlayRate = 1.0, const bool& StopAllMontage = true);
	void StopAllMontage(const float& weight = 0.25f);
	void ChangeRootMotionMode(ERootMotionMode::Type mode);

	UFUNCTION()
	virtual void OnMontageStarted(UAnimMontage* Montage) {};
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {};

	UFUNCTION()
	virtual void OnParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void InExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OutExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void FadeOutCharacter(const float Duration);

	UFUNCTION()
	virtual const float UpdateFadeOut(const float& Value);
	UFUNCTION()
	virtual void FinishFadeOut(class UNiagaraComponent* PSystem);

	UFUNCTION()
	virtual void GameStartAction() PURE_VIRTUAL(AGameCharacter::GameStartAction, );

protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> m_mesh;

	UPROPERTY(EditInstanceOnly, Category = "Unique Name")
	FString m_unique_name;

#pragma region Animation
	FName m_section_name;

	UPROPERTY()
	class UAnimInstance* m_anim_instance;

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
	TObjectPtr<AGameCharacter> m_actor_target;

	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<AGameCharacter> m_actor_execution_target;

	UPROPERTY(EditDefaultsOnly, Category = Parry)
	TObjectPtr<USphereComponent> m_sphere_parry;

	UPROPERTY(EditDefaultsOnly, Category = "Execution")
	TObjectPtr<USphereComponent> m_sphere_execution_back;

	UPROPERTY(EditDefaultsOnly, Category = "Execution")
	TObjectPtr<USphereComponent> m_sphere_execution_front;

	UPROPERTY()
	TArray<TObjectPtr<AActor>>	m_ignore_target;

	TArray<bool>	m_is_enable_execution;

	FName			m_attack_name_prev;
	bool			m_attack_success;
	bool			m_is_not_damage_mod;
	int32			m_lockon_index;

	EAttackType		m_attack_type_prev	= EAttackType::EATKT_None;
	EAttackStrength m_attack_strength	= EAttackStrength::EATKS_None;
	EBattlePose		m_battle_pose		= EBattlePose::EBP_Unoccupied;
#pragma endregion

#pragma region Interaction
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	USphereComponent* m_sphere_interaction;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* m_pawn_sensing;
#pragma endregion

#pragma region States
	UPROPERTY(BlueprintReadOnly, Category = "States")
	EDeathPose m_death_pose = EDeathPose::EDP_Alive;

	UPROPERTY(EditInstanceOnly, Category = "States")
	EEquipState	m_equip_state = EEquipState::EES_Unequipped;
#pragma endregion

#pragma region Weapon
	UPROPERTY()
	TObjectPtr<AWeapon_Actor> m_equiped_weapon_R;
	UPROPERTY()
	TObjectPtr<AWeapon_Actor> m_equiped_weapon_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowAccessPrivate = "true"))
	TArray<FName> m_equip_socket_name;
#pragma endregion

#pragma region Attribute
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> m_attribute;

	UPROPERTY(EditDefaultsOnly, Category = "Gold Amount")
	int32 m_total_gold;
#pragma endregion

#pragma region Sound
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> m_sound_hit;
#pragma endregion

#pragma region Particle
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* m_particle_hit = nullptr;
#pragma endregion

#pragma region DeathFadeOut
	TArray<class UMaterialInstanceDynamic*> m_dynamic_materials;
	FTimeline m_tl_fadeout;
#pragma endregion

	FTransform m_start_transform;
};
