// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Tutorial/TutorialVolumeBox.h"
#include "TutorialVolumeBox_CallOut.generated.h"

UCLASS()
class ELDENRING_API ATutorialVolumeBox_CallOut : public ATutorialVolumeBox
{
	GENERATED_BODY()
	
public:
	virtual const TArray<FTutorialPersistentData> GetCallOuts() const override final;
	virtual void SetCallOut(const TArray<FTutorialPersistentData>& CallOuts) override final;

protected:
	virtual void ReadyUp() override final;

	virtual void OnBoxBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override final;
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override final;

	virtual void AutoStart() override final;
	virtual void SaveTutorial() override final;

private:
	void SpawnCallOut();

	UFUNCTION()
	void ClearCallOut();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FText UI_CTA_TEXT_1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FText UI_CTA_TEXT_2;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	EInputActionType m_action;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bIsLargeKey;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bPressDone;
};
