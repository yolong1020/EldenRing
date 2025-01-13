// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/TimelineComponent.h"
#include "Camera/ERCameraTypes.h"
#include "CameraDirectingManager.generated.h"

UCLASS()
class ELDENRING_API UCameraDirectingManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

	void CamFocusRotate(TWeakObjectPtr<class APlayerController> Controller, TSharedRef<class USpringArmComponent> SprintArm);
	void CamExecutionFront();
	void CamExecutionBack();

private:
	void CameraDirecting(const EERCameraDirect& Type, const float& Curve);
	
	UFUNCTION()
	void FocusRotate(float Curve);
	UFUNCTION()
	void ExecutionFront(float Curve);
	UFUNCTION()
	void ExecutionBack(float Curve);

private:
	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_controller;

	UPROPERTY()
	TWeakObjectPtr<class AGameCharacter> m_actor_focus;

	UPROPERTY()
	TWeakObjectPtr<class USpringArmComponent> m_sprint_arm;

#pragma region TimeLine
	FTimeline m_timeline_camera_focus;
	FTimeline m_timeline_camera_execution_front;
	FTimeline m_timeline_camera_execution_back;
	FTimeline m_timeline_camera_rest;
#pragma endregion

#pragma region Curves
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_focus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_execution_front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_execution_back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_rest_end;
#pragma endregion
};
