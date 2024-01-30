// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimeLineComponent.h"
#include "Characters/CharacterTypes.h"
#include "Items/ItemProperties.h"
#include "Attribute_Delegates.h"
#include "AttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELDENRING_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	explicit UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override final;
	void InitUpdateCallback(const EOverlayStatType& Type, const FOnUpdateStat& Consume, const FOnUpdateRecoverStat& Recover);

	void RecoverHealth(float Amount);
	void RecoverStamina();

	void ReceiveDamage(float damage);
	void ConsumeStamina(float amount);


	UFUNCTION()
	void UpdateRecoverHealth(float curve_value);

	UFUNCTION()
	void UpdateRecoverMana(float curve_value);

	FORCEINLINE float GetHealthPercent()	{ return m_health_current / m_health_max; }
	FORCEINLINE float GetManaPercent()		{ return m_mana_current / m_mana_max; }
	FORCEINLINE float GetStaminaPercent()	{ return m_stamina_current / m_stamina_max; }

	FORCEINLINE float GetHealthCurrent()	{ return m_health_current; }
	FORCEINLINE float GetManaCurrent()		{ return m_mana_current; }
	FORCEINLINE float GetStaminaCurrent()	{ return m_stamina_current; }

	FORCEINLINE float GetHealthMax()		{ return m_health_max; }
	FORCEINLINE float GetManaMax()			{ return m_mana_max; }
	FORCEINLINE float GetStaminaMax()		{ return m_stamina_max; }

	void AddOffset(const TObjectPtr<class UItemObject> Item);
	void RemoveOffset(const TObjectPtr<class UItemObject> Item);

	void ResetHealthToFull();
	void ResetManaToFull();
	void ResetStaminaToFull();

	FORCEINLINE const bool IsAlive() const  { return m_health_current > 0.f; }

private:
	UFUNCTION()
	void StartHealthTL();

	UFUNCTION()
	void StartStaminaTL();

	UFUNCTION()
	void UpdateRecoverStamina(float curve_value);

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_health_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_health_max;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_mana_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_mana_max;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_stamina_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_stamina_max;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_regenerate;

	UPROPERTY()
	TWeakObjectPtr<class UDataTableManager> m_data_mgr;

	TArray<FTimeline>	m_tl_recovery;


	float m_health_goal;
	float m_mana_goal;

	TArray<FOnUpdateStat>									m_consume;
	TArray<FOnUpdateRecoverStat>							m_recover;
	TArray<FLatentActionInfo>								m_delay;
	TMap<TObjectPtr<class UItemObject>, FItemProperties>	m_offsets;
};
