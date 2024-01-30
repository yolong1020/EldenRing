// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectNextAction.generated.h"

UCLASS()
class ELDENRING_API UBTTask_SelectNextAction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SelectNextAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
