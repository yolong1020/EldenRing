// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_TrackingTarget.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTask_TrackingTarget::UBTTask_TrackingTarget()
{
}

EBTNodeResult::Type UBTTask_TrackingTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;

	npc_interface->TrackingTarget();

	return EBTNodeResult::Succeeded;
}
