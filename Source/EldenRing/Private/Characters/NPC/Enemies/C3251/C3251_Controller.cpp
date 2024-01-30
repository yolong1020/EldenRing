// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/C3251_Controller.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/Enemies/C3251/C3251.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Macro/DebugMacros.h"

AC3251_Controller::AC3251_Controller()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> board(TEXT("BlackboardData'/Game/Blueprint/Character/Enemy/C3251/BehaviorTree/BB_TreeGuard.BB_TreeGuard'"));
	if (board.Succeeded()) { m_bd = board.Object; }
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> tree(TEXT("BehaviorTree'/Game/Blueprint/Character/Enemy/C3251/BehaviorTree/BT_TreeGuard.BT_TreeGuard'"));
	if (tree.Succeeded()) { m_bt = tree.Object; }
}

void AC3251_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	StartAI();
}

void AC3251_Controller::OnUnPossess()
{
	Super::OnUnPossess();
}

void AC3251_Controller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	AC3251* character = Cast<AC3251>(GetCharacter());
	if (nullptr == character) return;

	character->OnMoveCompleted(Result);
}

void AC3251_Controller::StartAI()
{
	UBlackboardComponent* component = Blackboard.Get();

	if (UseBlackboard(m_bd, component))
	{
		bool result = RunBehaviorTree(m_bt);
		ensure(result);
	}
}

void AC3251_Controller::StopAI()
{
	UBehaviorTreeComponent* component = Cast<UBehaviorTreeComponent>(BrainComponent);
	CHECK_INVALID_PTR(component)

	component->StopTree();
}
