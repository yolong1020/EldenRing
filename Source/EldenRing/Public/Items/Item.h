// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment/EquipmentTypes.h"
#include "Item_Interface.h"
#include "ItemData.h"
#include "Components/TimelineComponent.h"
#include "Item.generated.h"

class  USphereComponent;
class  UNiagaraComponent;

struct FItemData;

UCLASS()
class ELDENRING_API AItem : public AActor, public IItem_Interface
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;
	virtual void SetSphereCollisionActivation(ECollisionEnabled::Type type);
	FORCEINLINE TObjectPtr<class UItemObject> GetItemObject() const { return m_item_object; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_sphere = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_effect_embers = nullptr;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FName					m_item_name;

	UPROPERTY()
	TObjectPtr<UItemObject>	m_item_object;

	FItemData m_item_data;
#pragma endregion

#pragma region FadeOut
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_fadeout;
	TArray<class UMaterialInstanceDynamic*> m_dynamic_materials;
	FTimeline m_tl_fadeout;
#pragma endregion
};
