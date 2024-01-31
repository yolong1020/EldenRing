EldenRing Project
===============================
포토폴리오 목적으로 엘든링의 내용들을 재해석하여 제작한 데모 프로젝트입니다. 

* Unreal Engine 5.3.2
* Microsoft Visual Studio Community 2022
* 2023.11.01 ~ 2024.01.27 (3개월)

Video
----------
[![Video Label](http://img.youtube.com/vi/ZO0a9uATi-o/0.jpg)](https://youtu.be/ZO0a9uATi-o)

Technical Overview
------------------

### 1. Character
![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049)
#### 1. GameCharacter
* Init Default Component & Values
* Death Process
  ```C++
  // ==========================================================
  // Step 01. 콜리전 비활성화 및 재생된 Montage Section 시간 도출.
  // ==========================================================
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

  // =============================================================================
  // Step 02. Controller 및 MovementComponent 비활성 후 캐릭터와 무기 Fade Out 진행.
  // =============================================================================
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
		true);
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
  	m_tl_fadeout.PlayFromStart(); }

  // ================================
  // Step 03. 이펙트 연출 후 엑터 파괴
  // ================================
  void AGameCharacter::UninitAndDestroy()
  {
  	DetachFromControllerPendingDestroy();
  	SetActorHiddenInGame(true);

  	LifeSpanExpired();
  }

  // ==============================
  // Step 04. 엑터가 소유한 무기 파괴
  // ==============================
  void AGameCharacter::Destroyed()
  {
  	Super::Destroyed();

  	if (m_equiped_weapon_R) { m_equiped_weapon_R->Destroy(); }
	if (m_equiped_weapon_L) { m_equiped_weapon_L->Destroy(); }
  }
  ```
