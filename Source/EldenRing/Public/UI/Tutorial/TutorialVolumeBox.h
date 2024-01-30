// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Input/InputActionType.h"
#include "UI/Tutorial/Tutorial_Interface.h"
#include "TutorialVolumeBox.generated.h"

class UBoxComponent;
class ATutorialManager;
class UPrimitiveComponent;
class UTutorialCallout;
struct FTimeline;

UCLASS()
class ELDENRING_API ATutorialVolumeBox : public AActor, public ITutorial_Interface
{
	GENERATED_BODY()
	
public:	
	ATutorialVolumeBox();
	virtual void Destroyed() override final;

	FORCEINLINE virtual const FTransform GetObjectTransform() const override final;
	FORCEINLINE virtual TSubclassOf<class ATutorialVolumeBox> GetClassType() const override final;

	virtual void SetObjectTransform(const FTransform& Transform) override final { SetActorTransform(Transform); }

protected:
	virtual void BeginPlay() override;
	virtual void ReadyUp() PURE_VIRTUAL(ATutorialVolumeBox::ReadyUp, );

	UFUNCTION()
	virtual void AutoStart() PURE_VIRTUAL(ATutorialVolumeBox::AutoStart, );
	virtual void SaveTutorial() PURE_VIRTUAL(ATutorialVolumeBox::SaveTutorial, );

private:
	UFUNCTION()
	virtual void OnBoxBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) PURE_VIRTUAL(ATutorialVolumeBox::OnBoxBeingOverlap, );

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) PURE_VIRTUAL(ATutorialVolumeBox::OnBoxEndOverlap, );

protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bUICreated;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

private:
	UPROPERTY()
	TObjectPtr<UBoxComponent> Volume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float AutoStartDelay;
};
