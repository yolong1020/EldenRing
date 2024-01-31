// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Decorators/BTDecorator_AttackRange.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("CanDashAttack");

}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return false;

	APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!pawn) return false;

	FVector dir = npc_interface->GetTargetLocation() - pawn->GetActorLocation();
	float len = dir.Length();
	float tracking_radius = npc_interface->GetTrackingRadius();
	float attack_radius = npc_interface->GetAttackRadius();
	ECombatAction_NPC type = (ECombatAction_NPC)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_ACTIONTYPE);
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_DASHBOUND, len <= tracking_radius);
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CLOSEBOUND, len <= attack_radius);

	return type == ECombatAction_NPC::ECAN_SingleAttack;
}
