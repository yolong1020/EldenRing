// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace Field::Function
{
	static FProperty* AddTransientField(UClass* Class, const FName& PropertyName, FFieldClass* PropertyClass);
}
