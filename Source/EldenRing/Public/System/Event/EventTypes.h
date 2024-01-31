#pragma once

UENUM(BlueprintType)
enum class EObserverEventType : uint8
{
	EOET_GraceDiscovered	UMETA(DisplayName = "Grace Discovered"),
	EOET_EnemyFelled		UMETA(DisplayName = "Enemy Felled"),
	EOET_UsingAssembly		UMETA(DisplayName = "Using AssemblyPoint"),
	EOET_SaveData			UMETA(DisplayName = "SaveData"),
	EOET_BossGate			UMETA(DisplayName = "BossGate"),
	EOET_Max				UMETA(DisplayName = "Max")
};