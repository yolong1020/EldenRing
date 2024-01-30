// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_MoveAction.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "AIController.h"
#include "Macro/DebugMacros.h"

UBTTask_MoveAction::UBTTask_MoveAction()
{
}

EBTNodeResult::Type UBTTask_MoveAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;
	
	UE_LOG(LogTemp, Warning, TEXT("Try MoveAction"))
	FDoAfterEndAnimation callback;
	callback.BindLambda([&]()->void { 
		UE_LOG(LogTemp, Warning, TEXT("Moving Finished"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
		});
	npc_interface->SetCallbackEndAnimation(callback);
	npc_interface->TryMoveAction();

	return EBTNodeResult::InProgress;
}
