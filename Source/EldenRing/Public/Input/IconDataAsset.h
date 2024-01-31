// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Input/InputIconData.h"
#include "IconDataAsset.generated.h"

UCLASS()
class ELDENRING_API UIconDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE TArray<FIconData>* const GetIconDataArr() { return &IconData; }
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TArray<FIconData> IconData;
};
