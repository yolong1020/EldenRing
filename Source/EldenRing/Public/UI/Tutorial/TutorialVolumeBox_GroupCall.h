// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Tutorial/TutorialVolumeBox.h"
#include "UI/Tutorial/TutorialData.h"
#include "TutorialVolumeBox_GroupCall.generated.h"

UCLASS()
class ELDENRING_API ATutorialVolumeBox_GroupCall : public ATutorialVolumeBox
{
	GENERATED_BODY()

public:
	explicit ATutorialVolumeBox_GroupCall();
	FORCEINLINE virtual const TArray<FTutorialPersistentData> GetCallOuts() const override final { return m_slides; }
	
	virtual void SetCallOut(const TArray<FTutorialPersistentData>& CallOuts) override final;

protected:
	virtual void ReadyUp() override final;

	virtual void OnBoxBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override final;
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override final;

	virtual void AutoStart() override final;
	virtual void SaveTutorial() override final;

private:
	void SpawnGroupCall();

	UFUNCTION()
	void ClearGroupCall();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TArray<FTutorialPersistentData> m_slides;
};
