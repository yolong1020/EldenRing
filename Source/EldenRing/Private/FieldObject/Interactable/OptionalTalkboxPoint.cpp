// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldObject/Interactable/OptionalTalkboxPoint.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/NPC/Vender/Vender.h"
#include "System/NPCInteractionManager.h"
#include "Characters/GameCharacter_Interface.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "Macro/DebugMacros.h"

void AOptionalTalkboxPoint::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("NpcMovePoint"));

	UShapeComponent* component = GetCollisionComponent();
	CHECK_INVALID_PTR(component)
	component->OnComponentBeginOverlap.AddDynamic(this, &AOptionalTalkboxPoint::OnOverlapPoint);
}

void AOptionalTalkboxPoint::OnOverlapPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!m_interact_mgr.Get())
	{
		UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
		CHECK_INVALID_PTR(instance)
		m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
		CHECK_INVALID_SMART_PTR(m_interact_mgr)
	}

	CHECK_INVALID_PTR(OtherActor)
	if (IsSavePoint && OtherActor->ActorHasTag(FName("Vender")))
	{
		IInteractor_Interface* interactor_interface = Cast<IInteractor_Interface>(OtherActor);
		CHECK_INVALID_PTR(interactor_interface)
		interactor_interface->SetOptionKey(OptionTalkName);

		IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(OtherActor);
		CHECK_INVALID_PTR(character_interface)
		character_interface->SetCharacterTransform(OtherActor->GetActorTransform());

		UShapeComponent* component = GetCollisionComponent();
		CHECK_INVALID_PTR(component)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
