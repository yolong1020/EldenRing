// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Animation/AnimMontage.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Components/BoxComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	PrimaryActorTick.bCanEverTick	= true;

	bUseControllerRotationYaw		= false;
	bUseControllerRotationPitch		= false;
	bUseControllerRotationRoll		= false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	m_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_spring_arm->SetupAttachment(GetRootComponent());
	m_spring_arm->TargetArmLength = 200.f;

	m_view_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_view_camera->SetupAttachment(m_spring_arm);

	//	TObjectptr
	USkeletalMeshComponent* mesh = GetMesh();
	m_hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	m_hair->SetupAttachment(mesh);
	m_hair->AttachmentName = FString("head");

	m_eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	m_eyebrows->SetupAttachment(mesh);
	m_eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlashCharacter::Tick(float DeltaTime)
{
}

#pragma region Bind Func

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveSide"), this, &ASlashCharacter::MoveSide);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::Interaction);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}

void ASlashCharacter::SetWeaponCollision(ECollisionEnabled::Type type)
{
	//if (nullptr != m_equiped_weapon)
	//{
	//	auto weapon = m_equiped_weapon->GetWeaponBox();
	//	if (nullptr != weapon)
	//	{
	//		weapon->SetCollisionEnabled(type);
	//	}
	//	m_equiped_weapon->m_ignore_actors.Empty();
	//}
}

#pragma endregion


#pragma region Input Func

void ASlashCharacter::MoveForward(float value)
{
	if (m_action_state != EActionState::EAS_Unoccupied) return;

	if (Controller && (value != 0.f))
	{
		// FVector forward = GetActorForwardVector();
		// AddMovementInput(forward, value);

		////////////////////////////////////////////

		// find out which way is forward
		const FRotator control_rotation = GetControlRotation();
		const FRotator Yaw_rotation(0.f, control_rotation.Yaw, 0.f);

		const FVector direction = FRotationMatrix(Yaw_rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void ASlashCharacter::MoveSide(float value)
{
	if (m_action_state != EActionState::EAS_Unoccupied) return;

	if (Controller && (value != 0.f))
	{
		// FVector right = GetActorRightVector();
		// AddMovementInput(right, value);

		////////////////////////////////////////////

		const FRotator control_rotation = GetControlRotation();
		const FRotator yaw_rotation(0.f, control_rotation.Yaw, 0.f);

		const FVector direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void ASlashCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ASlashCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void ASlashCharacter::Interaction()
{
	AWeapon_Actor* overlapping_weapon = Cast<AWeapon_Actor>(m_overlapping_item);
	if (nullptr != overlapping_weapon)
	{
		overlapping_weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		m_equip_state		= EEquipState::EES_EquippedOneHandedWeapon;
		m_overlapping_item  = nullptr;
		m_equiped_weapon	= overlapping_weapon;
	}
	else
	{
		if (nullptr == m_montage_equip) return;

		bool is_unequip = CanUnequip();

		PlayEquipMontage(is_unequip ? FName("Unequip") : FName("Equip"));
		m_equip_state		= is_unequip ? EEquipState::EES_Unequipped : EEquipState::EES_EquippedOneHandedWeapon;
		m_action_state		= EActionState::EAS_Equipping;
	}
}

void ASlashCharacter::Attack()
{
	if (EnableAttack())
	{
		PlayAttackMontage();
		m_action_state = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::EnableAttack()
{
	return (m_action_state		== EActionState::EAS_Unoccupied) &&
			(m_equip_state		!= EEquipState::EES_Unequipped);
}

bool ASlashCharacter::CanUnequip()
{
	return  (EActionState::EAS_Unoccupied	== m_action_state) &&
			(EEquipState::EES_Unequipped	!= m_equip_state);
} 

bool ASlashCharacter::CanEquip()
{
	if (nullptr == m_equiped_weapon) return false;

	return  (EActionState::EAS_Unoccupied	== m_action_state) &&
			(EEquipState::EES_Unequipped	== m_equip_state);
}

void ASlashCharacter::UnEquip()
{
	if (nullptr == m_equiped_weapon) return;

	m_equiped_weapon->AttachMashToSocket(GetMesh(), FName("SpineSocket"));
}

void ASlashCharacter::Equip()
{
	if (nullptr == m_equiped_weapon) return;

	m_equiped_weapon->AttachMashToSocket(GetMesh(), FName("RightHandSocket"));
}

void ASlashCharacter::FinishEquip()
{
	m_action_state = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (nullptr != anim_instance && nullptr != m_montage_attack)
	{
		anim_instance->Montage_Play(m_montage_attack);

		int32 selection		= FMath::RandRange(0, 1);
		FName section_name	= FName();

		switch (selection)
		{
		case 0: section_name = FName("Attack1"); break;
		case 1: section_name = FName("Attack2"); break;

		default: break;
		}

		anim_instance->Montage_JumpToSection(section_name, m_montage_attack);
	}
}

void ASlashCharacter::PlayEquipMontage(FName section_name)
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (nullptr != anim_instance && nullptr != m_montage_equip)
	{
		anim_instance->Montage_Play(m_montage_equip);
		anim_instance->Montage_JumpToSection(section_name, m_montage_equip);
	}
}

void ASlashCharacter::AttackEnd()
{
	m_action_state = EActionState::EAS_Unoccupied;
}


#pragma endregion
