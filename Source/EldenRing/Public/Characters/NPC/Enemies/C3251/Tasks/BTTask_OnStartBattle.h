// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_OnStartBattle.generated.h"

UCLASS()
class ELDENRING_API UBTTask_OnStartBattle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_OnStartBattle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
