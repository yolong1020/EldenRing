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

	// UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// if (!nav_sys) return EBTNodeResult::Failed;
	// 
	// FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGET);
	// FNavLocation pos;


	// if (nav_sys->GetRandomPointInNavigableRadius(origin, 500, pos))
	// {
	// 	OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS, pos.Location);
	// 	return EBTNodeResult::Succeeded;
	// }

	return EBTNodeResult::Succeeded;
}
