// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Characters/Component/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/SoundscapeManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Items/Item_Interface.h"
#include "System/ObserverManager.h"
#include "Macro/DebugMacros.h"

AGameCharacter::AGameCharacter()
{
	m_mesh = GetMesh();
	CHECK_INVALID_PTR(m_mesh);
	m_mesh->bWaitForParallelClothTask = true;

	//	Parry Sphere
	m_sphere_parry = CreateDefaultSubobject<USphereComponent>(TEXT("Parry Sphere"));
	m_sphere_parry->SetupAttachment(RootComponent);

	//	Interaction Sphere
	m_sphere_interaction = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	m_sphere_interaction->SetupAttachment(RootComponent);

	//	Execution Sphere
	m_is_enable_execution.SetNum((int32)EExecutionDirection::EED_Max);
	m_sphere_execution_front = CreateDefaultSubobject<USphereComponent>("Execution_Sphere_Front");
	m_sphere_execution_front->SetupAttachment(RootComponent);
	m_sphere_execution_back = CreateDefaultSubobject<USphereComponent>("Execution_Sphere_Back");
	m_sphere_execution_back->SetupAttachment(RootComponent);

	UCharacterMovementComponent* movement_component = GetCharacterMovement();
	CHECK_INVALID_PTR(movement_component);
	movement_component->bOrientRotationToMovement = true;
	movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

	bUseControllerRotationYaw	= false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll	= false;

	//	Attribute
	m_attribute	= CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

TArray<TSubclassOf<AWeapon_Actor>> AGameCharacter::GetWeaponClassTypes() const
{
	TArray<TSubclassOf<class AWeapon_Actor>> weapons;
	weapons.Reserve(2);

	if (m_equiped_weapon_R) { weapons.Add(m_equiped_weapon_R->GetClass()); }
	if (m_equiped_weapon_L) { weapons.Add(m_equiped_weapon_L->GetClass()); }

	return weapons;
}

const bool AGameCharacter::IsShowingWeapon() const
{
	if (!m_equiped_weapon_L && !m_equiped_weapon_R) return false;

	if (m_equiped_weapon_L && 
		m_equiped_weapon_L->GetWeaponType() == EWeaponType::EWT_StraightSword &&
		m_equiped_weapon_L->IsUsingWeapon()) return true;
	if (m_equiped_weapon_R && 
		m_equiped_weapon_R->GetWeaponType() == EWeaponType::EWT_StraightSword &&
		m_equiped_weapon_R->IsUsingWeapon()) return true;

	return false;
}

const bool AGameCharacter::IsAlive() const
{
	return m_attribute ? m_attribute->IsAlive() : false;
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_start_transform = GetTransform();
	m_anim_instance = GetMesh()->GetAnimInstance();
	CHECK_INVALID_PTR(m_anim_instance)
	m_anim_instance->OnMontageStarted.AddDynamic(this, &AGameCharacter::OnMontageStarted);
	m_anim_instance->OnMontageEnded.AddDynamic(this, &AGameCharacter::OnMontageEnded);

	//	Capsule Collision
	{
		UCapsuleComponent* capsule = GetCapsuleComponent();
		CHECK_INVALID_PTR(capsule)
		capsule->SetGenerateOverlapEvents(true);
		capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	}
	
	//	Mesh Collision
	{
		m_mesh->SetGenerateOverlapEvents(true);
		m_mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		m_mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
		m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	}

	//	Parry Sphere
	m_sphere_parry->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::OnParryOverlap);
	m_sphere_parry->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_sphere_parry->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	m_sphere_parry->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_sphere_parry->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	Execution Sphere
	m_sphere_execution_front->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::InExecutionOverlap);
	m_sphere_execution_front->OnComponentEndOverlap.AddDynamic(this, &AGameCharacter::OutExecutionOverlap);
	m_sphere_execution_back->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::InExecutionOverlap);
	m_sphere_execution_back->OnComponentEndOverlap.AddDynamic(this, &AGameCharacter::OutExecutionOverlap);
	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_execution_back->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_lockon_index = 0;

	// FadeOut Setting
	{
		TArray<UMaterialInterface*> materials = m_mesh->GetMaterials();
		for (int i = 0; i < materials.Num(); ++i)
		{
			if (!materials[i]) continue;

			UMaterialInstanceDynamic* dynamic = UMaterialInstanceDynamic::Create(materials[i], this);
			m_mesh->SetMaterial(i, dynamic);
			m_dynamic_materials.Add(dynamic);
		}
		m_dynamic_materials.Shrink();

		UCurveFloat* curve = LoadObject<UCurveFloat>(nullptr, TEXT("/Script/Engine.CurveFloat'/Game/Blueprint/Data/Curves/Common/C_FadeOut.C_FadeOut'"));
		CHECK_INVALID_PTR(curve)

		FOnTimelineFloat tl_callback;
		tl_callback.BindUFunction(this, FName("UpdateFadeOut"));
		m_tl_fadeout.AddInterpFloat(curve, tl_callback);
	}
}

void AGameCharacter::UninitAndDestroy()
{
	DetachFromControllerPendingDestroy();
	SetActorHiddenInGame(true);

	LifeSpanExpired();
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_tl_fadeout.TickTimeline(DeltaTime);
}

void AGameCharacter::Destroyed()
{
	Super::Destroyed();

	if (m_equiped_weapon_R) { m_equiped_weapon_R->Destroy(); }
	if (m_equiped_weapon_L) { m_equiped_weapon_L->Destroy(); }

	UGameInstance* instance = GetGameInstance();
	if (!instance) return;

	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	if (!observer_mgr) return;

	observer_mgr->UnregistObserverObject(this);
}

void AGameCharacter::SetExecutionEnable(const bool& IsFront, const bool& is_enable, AGameCharacter* const execution_target)
{
	m_is_enable_execution[(int32)(IsFront ? EExecutionDirection::EED_Front : EExecutionDirection::EED_Back)] = is_enable;
	m_actor_execution_target = execution_target;
}

void AGameCharacter::OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration)
{
	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_execution_back->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float fadeout_duration = Duration;
	if (fadeout_duration == 0)
	{
		TArray<FCompositeSection> sections = Montage->CompositeSections;
		for (auto section : sections)
		{
			if (section.SectionName == m_section_name)
			{
				const UAnimSequenceBase* sequence = section.GetLinkedSequence();
				fadeout_duration = sequence->GetPlayLength() - 0.2f;
				break;
			}
		}
	}

	FadeOutCharacter(fadeout_duration);
}

void AGameCharacter::OnTakeStunn()
{
	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGameCharacter::OnEndStunn()
{
	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGameCharacter::OnTakeExecutionEnd()
{
	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

const FVector AGameCharacter::GetBoneLocation(const FName& BoneName, const EBoneSpaces::Type& Space)
{
	if (nullptr == m_mesh) return FVector::Zero();
	return m_mesh->GetBoneLocation(BoneName, Space);
}

void AGameCharacter::SetParryCollision(const ECollisionEnabled::Type& type)
{
	CHECK_INVALID_PTR(m_sphere_parry);
	m_sphere_parry->SetCollisionEnabled(type);
}

void AGameCharacter::InitBodyAttackIgnoreCallback()
{

}

void AGameCharacter::PlayMontageSection(UAnimMontage* const Montage, const FName& SectionName, const float& PlayRate, const bool& StopAllMontage)
{
	if (nullptr == m_anim_instance || nullptr == Montage) return;

	m_section_name = SectionName;
	m_anim_instance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::Duration, 0.f, StopAllMontage);
	m_anim_instance->Montage_JumpToSection(SectionName, Montage);
}

void AGameCharacter::StopAllMontage(const float& weight)
{
	if (nullptr == m_anim_instance) return;

	m_anim_instance->StopAllMontages(0.25);
}

void AGameCharacter::ChangeRootMotionMode(ERootMotionMode::Type mode)
{
	if (nullptr == m_anim_instance) return;

	m_anim_instance->StopAllMontages(0.25);
	m_anim_instance->SetRootMotionMode(mode);

	bool allow_physics_rotation = (mode == ERootMotionMode::RootMotionFromEverything);
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = allow_physics_rotation;
}

void AGameCharacter::InExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter*	executor = Cast<AGameCharacter>(OtherActor);
	if (nullptr == executor || executor == this) { return; }

	if (FString("SphereDetect") != OtherComp->GetName())
	{
		executor->SetExecutionEnable(OverlappedComponent->GetName() == FString("Execution_Sphere_Front"), true, this);
	}
}

void AGameCharacter::OutExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* executor = Cast<AGameCharacter>(OtherActor);
	if (nullptr == executor) { return; }

	if (executor == this) { return; }

	executor->SetExecutionEnable(OverlappedComponent->GetName() == FString("Execution_Sphere_Front"), false);
}

const float AGameCharacter::UpdateFadeOut(const float& Value)
{
	float opacity = 1 - (m_tl_fadeout.GetPlaybackPosition() / m_tl_fadeout.GetTimelineLength());
	for (const auto& iter : m_dynamic_materials)
	{
		iter->SetScalarParameterValue(FName("Opacity"), opacity);
	}

	return opacity;
}

void AGameCharacter::FinishFadeOut(UNiagaraComponent* PSystem)
{
}

void AGameCharacter::FadeOutCharacter(const float Duration)
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}

	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MoveComp)
	{
		MoveComp->StopActiveMovement();
		MoveComp->DisableMovement();
	}

	UNiagaraSystem* particle_system = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Effects/Niagara/NS_FadeOut.NS_FadeOut'"));
	CHECK_INVALID_PTR(particle_system)

	UNiagaraComponent* particle_component = UNiagaraFunctionLibrary::SpawnSystemAttached(
		particle_system,
		RootComponent,
		FName("Spine"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);
	CHECK_INVALID_PTR(particle_component)
	particle_component->OnSystemFinished.AddDynamic(this, &AGameCharacter::FinishFadeOut);
	particle_component->SetVariableFloat(FName("Duration"), Duration);

	IItem_Interface* weapon_r = Cast<IItem_Interface>(m_equiped_weapon_R);
	if (weapon_r) { weapon_r->FadeOutItem(Duration); }

	IItem_Interface* weapon_l = Cast<IItem_Interface>(m_equiped_weapon_L);
	if (weapon_l) { weapon_l->FadeOutItem(Duration); }

	m_tl_fadeout.SetTimelineLength(Duration);
	m_tl_fadeout.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	m_tl_fadeout.Stop();
	m_tl_fadeout.PlayFromStart();
}
