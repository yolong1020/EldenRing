// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/NPC/Vender/Vender_Interface.h"
#include "Items/ItemVendor.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "UI/Communicate/CommunicateScriptData.h"
#include "GameData/SaveDatas.h"
#include "Vender.generated.h"

class UGroomComponent;
class UDataTable;
class UItemObject;
class ACameraActor;

struct FItemData;
struct FItemVendor;

UCLASS()
class ELDENRING_API AVender : public ANPC_Character, public IInteractor_Interface, public IVender_Interface
{
	GENERATED_BODY()

public :
	AVender();
	virtual void GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight) override final {};
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) override final;
	virtual void EquipWeaponToStart() override final {};
	virtual void SetTradeItems(const TArray<FInventorySaveData>& Items) override final;
	virtual void SetVenderName(const FString& Name) override final { m_name = Name; }

	UFUNCTION()
	void OnCompleteGraceDiscover();

	FORCEINLINE const bool& GetReadyInteraction()												{ return m_is_ready; }
	FORCEINLINE virtual const FString GetVenderName() const override final						{ return m_name; }
	FORCEINLINE virtual const FString GetInteractorName() const	override final					{ return m_name; }
	FORCEINLINE virtual const TObjectPtr<UDataTable> GetDialougeTable() const override final	{ return m_dialouge_table; }
	FORCEINLINE virtual TArray<TObjectPtr<UItemObject>>* const GetTradeItem() override final	{ return &m_slots; }
	virtual const TObjectPtr<ACameraActor> GetCameraActor() const override final;
	FORCEINLINE virtual const bool IsFirstInteract() const override final						{ return m_first_meet; }

	FORCEINLINE virtual void SetFirstInteract(const bool IsFirst) override final				{ m_first_meet = IsFirst; }
	FORCEINLINE virtual void DoFirstInteract() override final									{ m_first_meet = false; }

	FORCEINLINE virtual const int32 GetCommunicateIndex() const override final					{ return m_communicate_idx; }
	FORCEINLINE virtual void SetCommunicateIndex(const int32& Idx) override final				{ m_communicate_idx = Idx; };

	FORCEINLINE virtual TArray<FString> const GetCameras() const override final;
	FORCEINLINE virtual void SetCameras(const TArray<FString>& Cameras) override final;

	FORCEINLINE virtual void SetDialouge(const FCommunicateScriptData& Dialouge) override final	{ m_dialogue_current = Dialouge; }
	FORCEINLINE virtual const FCommunicateScriptData GetDialouge() const override final			{ return m_dialogue_current; }

	FORCEINLINE virtual const FString GetOptionKey() const										{ return m_option_key; }
	FORCEINLINE virtual void SetOptionKey(const FString& Key)									{ m_option_key = Key; }

	virtual void SetCurrentPoint(const FString& Name) override final;
	virtual const FString GetCurrentPointName() const override final;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void SaveInteractObjectData();

private:
	UFUNCTION()
	virtual void MoveToSavePoint();

private:
	UPROPERTY()
	TWeakObjectPtr<UCharacterMovementComponent> m_movement_component;

	UPROPERTY(EditDefaultsOnly, Category = Name)
	FString m_name;

	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> m_slots;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UTradeManager> m_trade_mgr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact Camera", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class ADirectCamera>> m_communicate_cameras;

	UPROPERTY()
	TObjectPtr<UDataTable> m_dialouge_table;

	UPROPERTY()
	FCommunicateScriptData m_dialogue_current;

	UPROPERTY(EditInstanceOnly, Category = "Move Point")
	TObjectPtr<class APatrolPoint> m_point_current;

	FItemVendor m_items;
	FString m_option_key;
	int32 m_communicate_idx;

	bool m_is_ready = false;
	bool m_first_meet = true;
};
