// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldPartition/DataLayer/ActorDataLayer.h"
#include "ERGameMode.generated.h"

UCLASS(Config = Game)
class ELDENRING_API AERGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AERGameMode();
	APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override final;
	void RequestRestartPlayer(TObjectPtr<AController> NewPlayer);

	FORCEINLINE const FActorDataLayer& GetDefaultData()		{ return DefaultData; }
	FORCEINLINE const FActorDataLayer& GetFieldObjectData() { return FieldObjectData; }
	FORCEINLINE const FActorDataLayer& GetLandScapeData()	{ return LandScapeData; }
	FORCEINLINE const FActorDataLayer& GetNavData()			{ return NavData; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Layer", meta = (AllowPrivateAccess = "true"))
	FActorDataLayer DefaultData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Layer", meta = (AllowPrivateAccess = "true"))
	FActorDataLayer FieldObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Layer", meta = (AllowPrivateAccess = "true"))
	FActorDataLayer LandScapeData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Layer", meta = (AllowPrivateAccess = "true"))
	FActorDataLayer NavData;
};
