// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/CharacterTypes.h"

DECLARE_DELEGATE_ThreeParams(FOnRefreshStatBar, const EOverlayStatType&, const float&, const bool& is_regenereate);
