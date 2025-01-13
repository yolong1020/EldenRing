// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/Tasks/BTTask_SelectNextAction.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/CommonFunctions.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTask_SelectNextAction::UBTTask_SelectNextAction()
{

}

EBTNodeResult::Type UBTTask_SelectNextAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(OwnerComp.GetAIOwner()->GetPawn());
	if (!npc_interface) return EBTNodeResult::Failed;

	bool is_acting = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ACTING);
	if (is_acting) return EBTNodeResult::Failed;

	bool is_in_dash   = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_DASHBOUND);
	bool is_in_single = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_SINGLEBOUND);
	bool is_in_combo  = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_COMBOBOUND);

	ECombatAction_NPC action = (ECombatAction_NPC)npc_interface->GetCombatAction();
	EGameDirection	  dir	 = (EGameDirection)npc_interface->GetTargetDirectionFromNPC();

	if (action == ECombatAction_NPC::ECAN_End) action = ECombatAction_NPC::ECAN_DashAttack;
	else
	{
		FName section_name = npc_interface->GetCurrentActionID();
		UE_LOG(LogTemp, Warning, TEXT("-> Current Action Name : %s"), *section_name.ToString());
		FString state = StaticEnum<ECombatAction_NPC>()->GetNameStringByValue(int64(action));
		UE_LOG(LogTemp, Warning, TEXT("-> Before Combo Action : %s"), *state);

		if (action == ECombatAction_NPC::ECAN_MoveAction)
		{
			if (is_in_single) { action = ECombatAction_NPC::ECAN_SingleAttack; }
			else 
			{
				if (dir == EGameDirection::EGD_Front) action = ECombatAction_NPC::ECAN_DashAttack;
				else								  action = ECombatAction_NPC::ECAN_MoveAction;
			}
		}
		else
		{
			if (is_in_combo && dir == EGameDirection::EGD_Left)
			{
				if (npc_interface->HasDerivedAttack())
				{
					action = ECombatAction_NPC::ECAN_ComboAttack;
				}
				else
				{
					action = ECombatAction_NPC::ECAN_SingleAttack;
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_SINGLECOMBO, true);
				}
			}
			else if (is_in_single && dir != EGameDirection::EGD_Back) 
			{ 
				action = ECombatAction_NPC::ECAN_SingleAttack; 
			}
			else
			{
				FString st = StaticEnum<ECombatAction_NPC>()->GetNameStringByValue(int64(action));

				action = ECombatAction_NPC::ECAN_MoveAction; 
			}
		}
	}

	FString state = StaticEnum<ECombatAction_NPC>()->GetNameStringByValue(int64(action));
	UE_LOG(LogTemp, Warning, TEXT("Lets Do : %s"), *state);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_ACTIONTYPE, (uint8)action);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, true);

	return EBTNodeResult::Succeeded;
}
