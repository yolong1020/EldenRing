// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_OnStartBattle.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTask_OnStartBattle::UBTTask_OnStartBattle()
{
}

EBTNodeResult::Type UBTTask_OnStartBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;

	UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!nav_sys) return EBTNodeResult::Failed;

	FDoAfterEndAnimation callback;
	callback.BindLambda([&]()->void { FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); });
	npc_interface->SetCallbackEndAnimation(callback);
	npc_interface->StartCombat();

	return EBTNodeResult::InProgress;
}
