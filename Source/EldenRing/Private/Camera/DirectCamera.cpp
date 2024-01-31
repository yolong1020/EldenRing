// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DirectCamera.h"

void ADirectCamera::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("DirectCamera"));
}
