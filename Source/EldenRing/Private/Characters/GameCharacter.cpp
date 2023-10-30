// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/LockOn/LockOnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Basic/DebugMacros.h"

AGameCharacter::AGameCharacter()
{
	m_mesh = GetMesh();
	CHECK_INVALID(m_mesh);

	m_mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	m_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_mesh->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	//	Parry Sphere
	m_sphere_parry = CreateDefaultSubobject<USphereComponent>(TEXT("Parry Sphere"));
	m_sphere_parry->SetupAttachment(RootComponent);

	//	Interaction Sphere
	m_sphere_interaction = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	m_sphere_interaction->SetupAttachment(RootComponent);

	//	Execution Sphere
	m_sphere_execution.SetNum(2);
	for (int i = 0; i < m_sphere_execution.Num(); ++i)
	{
		FString pre_fix  = "Execution Sphere_";
		FString post_fix = (i == 0) ? "Front" : "Back";

		FName name = FName(pre_fix + post_fix);
		m_sphere_execution[i] = CreateDefaultSubobject<USphereComponent>(name);
		m_sphere_execution[i]->SetupAttachment(RootComponent);
	}

	UCharacterMovementComponent* movement_component = GetCharacterMovement();
	CHECK_INVALID(movement_component);
	movement_component->bOrientRotationToMovement = true;
	movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

	bUseControllerRotationYaw	= false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll	= false;

	//	LockOn
	m_widget_lockon = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOn Component"));
	m_widget_lockon->SetupAttachment(RootComponent);

	//	Attribute
	m_attribute		= CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//	Parry Sphere
	m_sphere_parry->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::OnParryOverlap);
	m_sphere_parry->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_sphere_parry->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	Execution Sphere
	for (int i = 0; i < m_sphere_execution.Num(); ++i)
	{
		m_sphere_execution[i]->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::InExecutionOverlap);
		m_sphere_execution[i]->OnComponentEndOverlap.AddDynamic(this, &AGameCharacter::OutExecutionOverlap);
	}

	m_sphere_execution[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_lockon_index = 0;

	CHECK_INVALID(m_widget_lockon)
	m_widget_lockon->SetVisibility(false);
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameCharacter::Destroyed()
{
	Super::Destroyed();
}

float AGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AGameCharacter::SetExecutionEnable(const bool& is_enable, AGameCharacter* const execution_target)
{
	m_is_enable_execution	 = is_enable;
	m_actor_execution_target = execution_target;
}

void AGameCharacter::SetActiveLockOn(const bool& is_visible)
{
	CHECK_INVALID(m_widget_lockon)
	m_widget_lockon->SetVisibility(is_visible);
}

void AGameCharacter::OnTakeStunn()
{
	m_sphere_execution[0]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGameCharacter::OnEndStunn()
{
	m_sphere_execution[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGameCharacter::OnTakeExecutionEnd()
{
	m_sphere_execution[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

FVector AGameCharacter::GetBoneLocation(const FName& bone_name)
{
	if (nullptr == m_mesh) { return FVector::Zero(); }
	return m_mesh->GetBoneLocation(bone_name);
}

EGameDirection AGameCharacter::FindDirection(const AActor* standard, const FVector& target)
{
	if (nullptr == standard) return EGameDirection::EGD_None;

	const FVector forward  = standard->GetActorForwardVector();
	const FVector position = standard->GetActorLocation();
	
	//	Lower Impact Point to the Enemy's Actor Location.Z
	const FVector target_lowered(target.X, target.Y, position.Z);
	const FVector direction = (target_lowered - position).GetSafeNormal();

	//	Forward * ToHit = |Forward||ToHit| * cos(theta)
	//	|Forward| = 1, |ToHit| =1, so Foward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(forward, direction);

	//	Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);

	//	Convert from Radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//	If CrossProduct points down, Theta should be negative.
	const FVector cross = FVector::CrossProduct(forward, direction);
	if (cross.Z < 0) Theta *= -1.f;

	EGameDirection target_from = EGameDirection::EGD_Back;
	if (Theta >= -45.f && Theta < 45.f)
		target_from = EGameDirection::EGD_Front;
	else if (Theta >= -135.f && Theta < -45.f)
		target_from = EGameDirection::EGD_Left;
	else if (Theta >= 45.f && Theta < 135.f)
		target_from = EGameDirection::EGD_Right;

#ifdef _DEBUG
	UKismetSystemLibrary::DrawDebugArrow(this, position, position + cross * 100.f, 5.f, FColor::Red, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}

	UKismetSystemLibrary::DrawDebugArrow(this, position, position + forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, position, position + direction * 60.f, 5.f, FColor::Blue, 5.f);
#endif 

	return target_from;
}

void AGameCharacter::SetParryCollision(const ECollisionEnabled::Type& type)
{
	CHECK_INVALID(m_sphere_parry);
	m_sphere_parry->SetCollisionEnabled(type);
}

void AGameCharacter::PlayMontage(UAnimMontage* montage, const FName& section_name, const float& play_rate, const bool& stop_all_montage, const float& blend_out)
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (nullptr == anim_instance || nullptr == montage) return;

	if (stop_all_montage) { anim_instance->StopAllMontages(blend_out); }
	anim_instance->Montage_Play(montage, play_rate);
	anim_instance->Montage_JumpToSection(section_name, montage);
}

void AGameCharacter::StopAllMontage(const float& weight)
{
	UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
	if (nullptr == anim_instance) return;

	anim_instance->StopAllMontages(0.25);
}

void AGameCharacter::ChangeRootMotionMode(ERootMotionMode::Type mode)
{
	UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
	if (nullptr != anim_instance)
	{
		anim_instance->StopAllMontages(0.25);
		anim_instance->SetRootMotionMode(mode);

		bool allow_physics_rotation = (mode == ERootMotionMode::RootMotionFromEverything);
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = allow_physics_rotation;
	}
}

void AGameCharacter::OnParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CHECK_INVALID(OtherActor)

	AMeleeAttack_Actor* weapon	= Cast<AMeleeAttack_Actor>(OtherActor);
	if (nullptr == weapon) { return; }

	AGameCharacter* attacker	= Cast<AGameCharacter>(weapon->GetOwner());
	CHECK_INVALID(attacker)

	if (GetOwner() == attacker) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Parry Success"))
}

void AGameCharacter::InExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter*	executor = Cast<AGameCharacter>(OtherActor);
	if (nullptr == executor || executor == this) { return; }

	if (FString("SphereDetect") != OtherComp->GetName())
	{
		executor->SetExecutionEnable(true, this);
	}
}

void AGameCharacter::OutExecutionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* executor = Cast<AGameCharacter>(OtherActor);
	if (nullptr == executor) { return; }

	if (executor == this) { return; }

	executor->SetExecutionEnable(false);
}
