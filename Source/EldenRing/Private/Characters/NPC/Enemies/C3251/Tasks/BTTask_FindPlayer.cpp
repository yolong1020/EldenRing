// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_FindPlayer.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Macro/DebugMacros.h"

UBTTask_FindPlayer::UBTTask_FindPlayer()
{
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* controlling_pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!controlling_pawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!nav_sys) return EBTNodeResult::Failed;

	//FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_FINDTARGET);
	//FNavLocation pos;
	//
	//if (nav_sys->GetRandomPointInNavigableRadius(origin, 500, pos))
	//{
	//	//OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS, pos.Location);
	//	return EBTNodeResult::Succeeded;
	//}

	return EBTNodeResult::Failed;
}
