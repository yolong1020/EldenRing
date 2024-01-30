// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Services/BTService_Detect.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "AIController.h"
#include "Macro/DebugMacros.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* npc = OwnerComp.GetAIOwner()->GetPawn();
	CHECK_INVALID_PTR(npc)

	FVector location = npc->GetActorLocation();

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(npc);
	CHECK_INVALID_PTR(npc_interface)

	FVector dir = npc_interface->GetTargetLocation() - location;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_DASHBOUND, dir.Size() <= npc_interface->GetTrackingRadius());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_SINGLEBOUND, dir.Size() <= npc_interface->GetConfrontingRadius());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_COMBOBOUND, dir.Size() <= npc_interface->GetAttackRadius());
}
