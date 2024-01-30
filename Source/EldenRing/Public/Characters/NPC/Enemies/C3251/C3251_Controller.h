// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C3251_Controller.generated.h"

UCLASS()
class ELDENRING_API AC3251_Controller : public AAIController
{
	GENERATED_BODY()
	
public:
	AC3251_Controller();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void StartAI();
	void StopAI();

private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> m_bt;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> m_bd;
};
