// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Birds.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class BASIC_API ABirds : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirds();
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float input_value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* m_bird_mapping_context = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* m_move_action = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* m_look_action = nullptr;

	void Turn(float value);
	void LookUp(float value);

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_spring_arm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_view_camera;
};
