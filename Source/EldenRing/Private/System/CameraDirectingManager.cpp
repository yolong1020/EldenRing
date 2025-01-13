// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CameraDirectingManager.h"

void UCameraDirectingManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// FOnTimelineFloat tl_focus_callback;
	// FOnTimelineFloat tl_execution_front;
	// FOnTimelineFloat tl_execution_back;
	// FOnTimelineFloat tl_rest;
	// 
	// tl_focus_callback.BindUFunction(this, FName("FocusRotate"));
	// tl_execution_front.BindUFunction(this, FName("ExecutionFront"));
	// tl_execution_back.BindUFunction(this, FName("ExecutionBack"));
	// tl_rest.BindUFunction(this, FName("Rest"));
	// 
	// m_timeline_camera_focus.AddInterpFloat(m_curve_camera_focus, tl_focus_callback);
	// m_timeline_camera_execution_front.AddInterpFloat(m_curve_camera_execution_front, tl_execution_front);
	// m_timeline_camera_execution_back.AddInterpFloat(m_curve_camera_execution_back, tl_execution_back);
	// m_timeline_camera_rest.AddInterpFloat(m_curve_camera_rest_end, tl_rest);
}

void UCameraDirectingManager::CameraDirecting(const EERCameraDirect& Type, const float& Curve)
{
	// FTimeline* timeline = nullptr;
	// switch (Type)
	// {
	// case EERCameraDirect::ECD_Focus: timeline = &m_timeline_camera_focus;			break;
	// case EERCameraDirect::ECD_Front: timeline = &m_timeline_camera_execution_front;	break;
	// case EERCameraDirect::ECD_Back:  timeline = &m_timeline_camera_execution_back;	break;
	// }

	// if (!m_actor_target) { timeline->Stop(); return; }
	   
	// if (type != ECameraDirect::ECD_Focus) { m_spring_arm->TargetArmLength = m_spring_arm_length - (curve_value * 150); }
	   
	// FRotator rotator_current = m_player_controller->GetControlRotation();
	// FRotator rotator_direction = UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
	   
	// FRotator rotator_goal = FRotator(type == ECameraDirect::ECD_Focus ? rotator_current.Pitch : 340.f, rotator_direction.Yaw, rotator_direction.Roll);
	// FRotator rotation_calc = UKismetMathLibrary::RInterpTo(m_player_controller->GetControlRotation(), rotator_goal, curve_value, 10);
	   
	// m_player_controller->SetControlRotation(FRotator(rotator_current.Pitch, rotation_calc.Yaw, rotation_calc.Roll));
	   
	// if (type == ECameraDirect::ECD_Focus && rotation_calc == rotator_goal) { m_timeline_camera_focus.Stop(); }
}

void UCameraDirectingManager::FocusRotate(float Curve)
{
	CameraDirecting(EERCameraDirect::ECD_Focus, Curve);
}

void UCameraDirectingManager::ExecutionFront(float Curve)
{
	CameraDirecting(EERCameraDirect::ECD_Front, Curve);
}

void UCameraDirectingManager::ExecutionBack(float Curve)
{
	CameraDirecting(EERCameraDirect::ECD_Back, Curve);
}
