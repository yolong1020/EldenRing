// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class  UItemObject;
struct FInventoryTile;

DECLARE_DELEGATE_OneParam(FItemOnRemoved, UItemObject*);

DECLARE_DELEGATE_OneParam(FItemOnAddAtEmpty, UItemObject*);
DECLARE_DELEGATE_TwoParams(FItemOnAdd, UItemObject*, const FInventoryTile&);
