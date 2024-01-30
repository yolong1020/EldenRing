// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_SingleAttack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "AIController.h"
#include "Macro/DebugMacros.h"

UBTTask_SingleAttack::UBTTask_SingleAttack()
{
}

EBTNodeResult::Type UBTTask_SingleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("Try SingleAttack"))
	FDoAfterEndAnimation callback;
	callback.BindLambda([&]()->void { 
		UE_LOG(LogTemp, Warning, TEXT("SingleAttack Finished"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_SINGLECOMBO, false);
		});
	npc_interface->SetCallbackEndAnimation(callback);
	npc_interface->TrySingleAttack();

	return EBTNodeResult::InProgress;
}
