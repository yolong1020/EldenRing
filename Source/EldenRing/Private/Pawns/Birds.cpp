// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Birds.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABirds::ABirds()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	// Capsule = GetRootComponent() = RootComponent -> 모두 같은 루트입니다.
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	m_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_spring_arm->SetupAttachment(RootComponent);

	m_view_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_view_camera->SetupAttachment(m_spring_arm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABirds::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* player_controller = Cast<APlayerController>(GetController());
	if (player_controller)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(m_bird_mapping_context, 0);
		}
	}
}

void ABirds::MoveForward(float input_value)
{
	if ((nullptr != Controller) && (0.f != input_value))
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, input_value);
	}
}

void ABirds::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABirds::LookUp(float value)
{
	AddControllerYawInput(value);
}

void ABirds::Move(const FInputActionValue& value)
{
	//const bool cur_value = value.Get<bool>();

	//if (cur_value)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IA_MOVE triggered"));
	//}
	const float cur_value = value.Get<float>();

	if ((nullptr != Controller) && (0.f != cur_value))
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, cur_value);
	}
}

void ABirds::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();

	if ((nullptr != Controller))
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

// Called every frame
void ABirds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABirds::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 1. Enhanced Input
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(m_move_action, ETriggerEvent::Triggered, this, &ABirds::Move);
		EnhancedInputComponent->BindAction(m_look_action, ETriggerEvent::Triggered, this, &ABirds::Look);
	}
	 
	// 2. Player Input
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABirds::MoveForward);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &ABirds::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABirds::LookUp);
}

