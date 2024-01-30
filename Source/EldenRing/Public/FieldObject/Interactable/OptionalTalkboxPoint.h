// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldObject/PatrolPoint/PatrolPoint.h"
#include "OptionalTalkboxPoint.generated.h"

UCLASS()
class ELDENRING_API AOptionalTalkboxPoint : public APatrolPoint
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditInstanceOnly, Category = "TalkBox")
	FString OptionTalkName;

	UPROPERTY(EditInstanceOnly, Category = "SavePoint")
	bool IsSavePoint;

	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager> m_interact_mgr;
};
