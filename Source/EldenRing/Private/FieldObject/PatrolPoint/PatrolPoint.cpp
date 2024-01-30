// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/PatrolPoint/PatrolPoint.h"
#include "Components/SphereComponent.h"
#include "Macro/DebugMacros.h"

void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("PatrolPoint"));

	UShapeComponent* component = GetCollisionComponent();
	CHECK_INVALID_PTR(component)

	component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
