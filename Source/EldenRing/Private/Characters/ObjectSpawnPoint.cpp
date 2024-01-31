// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ObjectSpawnPoint.h"

void AObjectSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("SpawnPoint"));
}
