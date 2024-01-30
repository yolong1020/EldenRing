// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_DashAttack.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTask_DashAttack::UBTTask_DashAttack()
{
}

EBTNodeResult::Type UBTTask_DashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;

	FDoAfterEndAnimation callback;
	callback.BindLambda([&]()->void { 
		UE_LOG(LogTemp, Warning, TEXT("Call Dash Finished"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
		});
	npc_interface->SetCallbackEndAnimation(callback);
	npc_interface->TryDashAttack();

	return EBTNodeResult::InProgress;
}
