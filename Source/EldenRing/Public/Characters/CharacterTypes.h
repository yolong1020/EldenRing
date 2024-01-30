#pragma once

//	Common ------------------------------------------------------

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	EES_Unequipped				UMETA(DisplayName = "Unequipped"),
	EES_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	EES_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EBattlePose : uint8
{
	EBP_Unoccupied	UMETA(DisplayName = "Unoccupied"),
	EBP_Attack		UMETA(DisplayName = "Attack Battle Pose"),
	EBP_Defense		UMETA(DisplayName = "Defense Battle Pose")
};

UENUM(BlueprintType)
enum class EGameDirection : uint8
{
	EGD_None	UMETA(DisplayName = "None"),
	EGD_Back	UMETA(DisplayName = "At Back"),
	EGD_Front	UMETA(DisplayName = "At Front"),
	EGD_Left	UMETA(DisplayName = "At Left"),
	EGD_Right	UMETA(DisplayName = "At Right")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EATKT_None			UMETA(DisplayName = "None"),
	EATKT_Attack		UMETA(DisplayName = "Attack"),
	EATKT_DashAttack	UMETA(DisplayName = "Dash Attack"),
	EATKT_JumpAttack	UMETA(DisplayName = "Jump Attack"),
	EATKT_ParryAttack	UMETA(DisplayName = "Parry Attack")
};

UENUM(BlueprintType)
enum class EAttackStrength : uint8
{
	EATKS_None		UMETA(DisplayName = "None"),
	EATKS_Normal	UMETA(DisplayName = "Normal Attack"),
	EATKS_Strong	UMETA(DisplayName = "Strong Attack")
};

UENUM(BlueprintType)
enum class EOverlayStatType : uint8
{
	EOST_Health		UMETA(DisplayName = "Health"),
	EOST_Mana		UMETA(DisplayName = "Mana"),
	EOST_Stamina	UMETA(DisplayName = "Stamina"),
	EOST_End		UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EHitCycleType : uint8
{
	EHCT_None	UMETA(DisplayName = "None"),
	EHCT_Even	UMETA(DisplayName = "Even"),
	EHCT_Odd	UMETA(DisplayName = "Odd")
};

UENUM(BlueprintType)
enum class EDirectionState : uint8
{
	EDS_DirectionOff	UMETA(DisplayName = "DirectionOff"),
	EDS_DirectionOn		UMETA(DisplayName = "DirectionOn")
};

UENUM(BlueprintType)
enum class EWeaponEquipHand : uint8
{
	EWEH_Right	UMETA(DisplayName = "Right Hand"),
	EWEH_Left	UMETA(DisplayName = "Left Hand")
};

UENUM(BlueprintType)
enum class EExecutionDirection : uint8
{
	EED_Front	UMETA(DisplayName = "Front Execution"),
	EED_Back	UMETA(DisplayName = "Back Execution"),
	EED_Max		UMETA(DisplayName = "Max")
};

//	Player ------------------------------------------------------

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied		UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking		UMETA(DisplayName = "Attacking"),
	EAS_Equipping		UMETA(DisplayName = "Equipping"),
	EAS_Sprinting		UMETA(DisplayName = "Sprinting"),
	EAS_Dodgeing		UMETA(DisplayName = "Dodgeing"),
	EAS_Turning			UMETA(DisplayName = "Turning"),
	EAS_Guarding		UMETA(DisplayName = "Guarding"),
	EAS_Knockbacking	UMETA(DisplayName = "Knockbacking"),
	EAS_Parry			UMETA(DisplayName = "Parry"),
	EAS_HitReact		UMETA(DisplayName = "HitReact"),
	EAS_BlockReact		UMETA(DisplayName = "BlockReact"),
	EAS_GuardReact		UMETA(DisplayName = "GuardReact"),
	EAS_Execution		UMETA(DisplayName = "Execution"),
	EAS_Consume			UMETA(DisplayName = "Consume"),
	EAS_Sit				UMETA(DisplayName = "Sit")
};

UENUM(BlueprintType)
enum class EGroundState : uint8
{
	EGS_Unoccupied	UMETA(DisplayName = "Unoccupied"),
	EGS_Walk		UMETA(DisplayName = "Walk"),
	EGS_Jog			UMETA(DisplayName = "Jog"),
	EGS_Sprint		UMETA(DisplayName = "Sprint")
};

UENUM(BlueprintType)
enum class EJumpState : uint8
{
	EJS_Unoccupied	UMETA(DisplayName = "Unoccupied"),
	EJS_JumpStart	UMETA(DisplayName = "JumpStart"),
	EJS_InAir		UMETA(DisplayName = "InAir"),
	EJS_Landing		UMETA(DisplayName = "Landing")
};

UENUM(BlueprintType)
enum class ELockOnState : uint8
{
	ELOS_LockOff	UMETA(DisplayName = "LockOff"),
	ELOS_LockOn		UMETA(DisplayName = "LockOn")
};

//	NPC --------------------------------------------------

UENUM(BlueprintType)
enum class EVigilanceState : uint8
{
	EVS_Repose		UMETA(DisplayName = "Repose"),
	EVS_Vigilance	UMETA(DisplayName = "Vigilance")
};

UENUM(BlueprintType)
enum class EActionState_NPC : uint8
{
	EASN_Unoccupied				UMETA(DisplayName = "Unoccupied"),
	EASN_Swap					UMETA(DisplayName = "Swap"),
	EASN_Watching				UMETA(DisplayName = "Watching"),
	EASN_Resting				UMETA(DisplayName = "Resting"),
	EASN_Patrolling				UMETA(DisplayName = "Patrolling"),
	EASN_ReturningPointObject	UMETA(DisplayName = "Returning PointObject"),
	EASN_Chasing				UMETA(DisplayName = "Chasing"),
	EASN_Attacking				UMETA(DisplayName = "Attacking"),
	EASN_Confronting			UMETA(DisplayName = "Confronting"),
	EASN_Turn					UMETA(DisplayName = "Turning"),
	EASN_Stunning				UMETA(DisplayName = "Stunning"),
	EASN_TakeExecution			UMETA(DisplayName = "TakeExecution"),
	EASN_HitReact				UMETA(DisplayName = "HitReact"),
	EASN_GuardReact				UMETA(DisplayName = "GuardReact")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive		UMETA(DisplayName = "Alive"),
	EDP_Death_Front	UMETA(DisplayName = "Death_Front"),
	EDP_Death_Back	UMETA(DisplayName = "Death_Back")
};

UENUM(BlueprintType)
enum class ECombatAction_NPC : uint8
{
	ECAN_DashAttack		UMETA(DisplayName = "Dash Attack"),
	ECAN_ComboAttack	UMETA(DisplayName = "Combo Attack"),
	ECAN_SingleAttack	UMETA(DisplayName = "Single Attack"),
	ECAN_MoveAction		UMETA(DisplayName = "Move Action"),
	ECAN_End			UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class ECombatBound : uint8
{
	ECB_None		UMETA(DisplayName = "None"),
	ECB_Tracking	UMETA(DisplayName = "Tracking Bound"),
	ECB_Confront	UMETA(DisplayName = "Confronting Bound"),
	ECB_Attack		UMETA(DisplayName = "Attacking Bound")
};